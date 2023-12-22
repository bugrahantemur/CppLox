#include "./interpreter.h"

#include <string>
#include <variant>

#include "./types/expression.h"
#include "./types/object.h"
#include "./types/statement.h"
#include "./types/token.h"
#include "./utils/box.h"
#include "./utils/error.h"

namespace {
template <typename... Objects>
auto check_number_operand(Token const& token, Objects... operands) -> void {
  std::array const ops{operands...};
  if (std::all_of(std::begin(ops), std::end(ops), [](Object const& obj) {
        return !std::holds_alternative<double>(obj);
      })) {
    throw RuntimeError{token.line_, sizeof...(operands) > 1
                                        ? "Operands must be numbers."
                                        : "Operand must be a number."};
  }
}

template <typename OStream>
struct Put {
  Put(OStream& out) : out_{out} {}

  auto operator()(std::string const& str) -> OStream& { return put(str); }

  auto operator()(double const number) -> OStream& { return put(number); }

  auto operator()(bool const b) -> OStream& {
    return b ? put("true") : put("false");
  }

  auto operator()(std::monostate) -> OStream& { return put("nil"); }

 private:
  template <typename T>
  auto put(T const& obj) -> OStream& {
    out_ << obj << '\n';
    return out_;
  }

  OStream& out_;
};

struct ExpressionEvaluator {
  explicit ExpressionEvaluator(Interpreter& interpreter)
      : interpreter_{interpreter} {}

  [[nodiscard]] auto operator()(std::monostate) -> Object { return {}; }

  [[nodiscard]] auto operator()(LiteralExpression const& expr) -> Object {
    return expr.value_;
  }

  [[nodiscard]] auto operator()(VariableExpression const& expr) -> Object {
    if (std::optional<Object> const value =
            interpreter_.environment_.get(expr.name_.lexeme_)) {
      return *value;
    }
    try {
      return interpreter_.environment_.get(expr.name_.lexeme_);
    } catch (std::out_of_range const&) {
      throw RuntimeError{expr.name_.line_,
                         "Undefined variable '" + expr.name_.lexeme_ + "'."};
    }
  }

  [[nodiscard]] auto operator()(Box<GroupingExpression> const& expr) -> Object {
    return std::visit(*this, expr->expression_);
  }

  [[nodiscard]] auto operator()(Box<UnaryExpression> const& expr) -> Object {
    Object const right{std::visit(*this, expr->right_)};

    Token const& op{expr->op_};
    TokenType const& op_type{op.type_};

    if (op_type == TokenType::MINUS) {
      check_number_operand(op, right);
      return -std::get<double>(right);
    }

    auto const is_truthy = [](Object const& obj) {
      if (std::holds_alternative<std::monostate>(obj)) {
        return false;
      }

      if (std::holds_alternative<bool>(obj)) {
        return std::get<bool>(obj);
      }

      return true;
    };

    if (op_type == TokenType::BANG) {
      return !is_truthy(right);
    }

    return std::monostate{};
  }

  [[nodiscard]] auto operator()(Box<BinaryExpression> const& expr) -> Object {
    Object const left{std::visit(*this, expr->left_)};
    Object const right{std::visit(*this, expr->right_)};

    Token const& op{expr->op_};
    TokenType const& op_type{op.type_};

    if (op_type == TokenType::MINUS) {
      check_number_operand(op, left, right);
      return std::get<double>(left) - std::get<double>(right);
    }
    if (op_type == TokenType::SLASH) {
      check_number_operand(op, left, right);
      return std::get<double>(left) / std::get<double>(right);
    }
    if (op_type == TokenType::STAR) {
      check_number_operand(op, left, right);
      return std::get<double>(left) * std::get<double>(right);
    }
    if (op_type == TokenType::PLUS) {
      if (std::holds_alternative<double>(left) &&
          std::holds_alternative<double>(right)) {
        return std::get<double>(left) + std::get<double>(right);
      }
      if (std::holds_alternative<std::string>(left) &&
          std::holds_alternative<std::string>(right)) {
        return std::get<std::string>(left) + std::get<std::string>(right);
      }
      throw RuntimeError{op.line_,
                         "Operands must be two numbers or two strings."};
    }
    if (op_type == TokenType::GREATER) {
      check_number_operand(op, left, right);
      return std::get<double>(left) > std::get<double>(right);
    }
    if (op_type == TokenType::GREATER_EQUAL) {
      check_number_operand(op, left, right);
      return std::get<double>(left) >= std::get<double>(right);
    }
    if (op_type == TokenType::LESS) {
      check_number_operand(op, left, right);
      return std::get<double>(left) < std::get<double>(right);
    }
    if (op_type == TokenType::LESS_EQUAL) {
      check_number_operand(op, left, right);
      return std::get<double>(left) <= std::get<double>(right);
    }
    if (op_type == TokenType::BANG_EQUAL) {
      return left != right;
    }
    if (op_type == TokenType::EQUAL_EQUAL) {
      return left == right;
    }
    // Unreachable
    return std::monostate{};
  }

  [[nodiscard]] auto operator()(Box<AssignmentExpression> const& expr)
      -> Object {
    Object const value = std::visit(*this, expr->value_);

    try {
      interpreter_.environment_.assign(expr->name_.lexeme_, value);
      return value;
    } catch (std::out_of_range const&) {
      throw RuntimeError(expr->name_.line_,
                         "Undefined variable '" + expr->name_.lexeme_ + "'.");
    }
  }

  Interpreter& interpreter_;
};

struct StatementExecutor {
  explicit StatementExecutor(Interpreter& interpreter)
      : interpreter_{interpreter} {}

  auto operator()(std::monostate) -> void {}

  auto operator()(ExpressionStatement const& stmt) -> void {
    static_cast<void>(
        std::visit(ExpressionEvaluator{interpreter_}, stmt.expression_));
  }

  auto operator()(PrintStatement const& stmt) -> void {
    Object const value{
        std::visit(ExpressionEvaluator{interpreter_}, stmt.expression_)};
    std::visit(Put{std::cout}, value);
  }

  auto operator()(VariableStatement const& stmt) -> void {
    Object const value{
        !std::holds_alternative<std::monostate>(stmt.initializer_)
            ? std::visit(ExpressionEvaluator{interpreter_}, stmt.initializer_)
            : std::monostate{}};

    interpreter_.environment_.define(stmt.name_, value);
  }

  auto operator()(Box<BlockStatement> const& stmt) -> void {
    // Create new environment with the current environment as its enclosing
    // environment
    decltype(interpreter_.environment_) env{&interpreter_.environment_};

    // Create interpreter for the block with the new environment
    Interpreter block_interpreter{env};

    // Execute statements in the block with the new environment
    for (Statement const& statement : stmt->statements_) {
      std::visit(StatementExecutor{block_interpreter}, statement);
    }
  }

  Interpreter& interpreter_;
};
}  // namespace

auto Interpreter::interpret(std::vector<Statement const> const& statements)
    -> void {
  for (auto const& stmt : statements) {
    std::visit(StatementExecutor{*this}, stmt);
  }
}
