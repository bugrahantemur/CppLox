#include "./interpreter.hpp"

#include <concepts>
#include <string>
#include <variant>

#include "./builtins.hpp"
#include "./types/expression.hpp"
#include "./types/function.hpp"
#include "./types/object.hpp"
#include "./types/statement.hpp"
#include "./types/token.hpp"
#include "./utils/box.hpp"
#include "./utils/error.hpp"

namespace {
struct Return {
  Object value_;
};

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

struct Equality {
  auto operator()(std::monostate, std::monostate) -> bool { return true; }
  auto operator()(bool left, bool right) -> bool { return left == right; }
  auto operator()(double left, double right) -> bool { return left == right; }
  auto operator()(std::string const& left, std::string const& right) -> bool {
    return left == right;
  }
  auto operator()(Box<LoxFunction> const& left, LoxFunction const& right)
      -> bool {
    // TODO
    return false;
  }

  template <typename T, typename U>
  auto operator()(T const& left, U const& right) -> bool {
    return false;
  }
};

auto is_equal(Object const& left, Object const& right) -> bool {
  return std::visit(Equality{}, left, right);
}

template <typename OStream>
struct Put {
  Put(OStream& out) : out_{out} {}

  auto operator()(std::string const& str) -> OStream& { return put(str); }

  auto operator()(double const number) -> OStream& { return put(number); }

  auto operator()(bool const b) -> OStream& {
    return b ? put("true") : put("false");
  }

  auto operator()(Box<LoxFunction> const& func) -> OStream& {
    return put("<fn " + func->declaration_.name_.lexeme_ + ">");
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

auto is_truthy(Object const& obj) -> bool {
  struct Truth {
    auto operator()(std::monostate) -> bool { return false; }

    auto operator()(bool b) -> bool { return b; }

    auto operator()(double number) -> bool { return true; }

    auto operator()(std::string const& str) -> bool { return true; }

    auto operator()(Box<LoxFunction> const& func) -> bool { return true; }
  };

  return std::visit(Truth{}, obj);
}

struct UncallableError : public std::exception {};
struct Arity {
  auto operator()(Box<LoxFunction> const& func) -> std::size_t {
    return func->declaration_.params_.size();
  }

  template <typename T>
  auto operator()(T const& t) -> std::size_t {
    throw UncallableError{};
  }
};

struct Call {
  [[nodiscard]] auto operator()(Box<LoxFunction>& func) -> Object {
    auto const env{std::make_shared<Environment>(func->closure_)};

    std::size_t const arity{std::visit(Arity{}, Object{func})};

    for (std::size_t i = 0; i < arity; ++i) {
      env->define(func->declaration_.params_.at(i).lexeme_, args_.at(i));
    }

    try {
      Interpreter::interpret(func->declaration_.body_, resolution_, env);
    } catch (Return const& ret) {
      return ret.value_;
    }

    return std::monostate{};
  }

  template <typename T>
  [[nodiscard]] auto operator()(T const& t) -> Object {
    throw UncallableError{};
  }

  std::shared_ptr<Environment> environment_;
  std::unordered_map<Token, std::size_t> const& resolution_;
  std::vector<Object> const& args_;
};

struct ExpressionEvaluator {
  std::shared_ptr<Environment> environment_;
  std::unordered_map<Token, std::size_t> const& resolution_;

  [[nodiscard]] auto operator()(std::monostate) -> Object { return {}; }

  [[nodiscard]] auto operator()(LiteralExpression const& expr) -> Object {
    if (std::holds_alternative<bool>(expr.value_)) {
      return std::get<bool>(expr.value_);
    }
    if (std::holds_alternative<double>(expr.value_)) {
      return std::get<double>(expr.value_);
    }
    if (std::holds_alternative<std::string>(expr.value_)) {
      return std::get<std::string>(expr.value_);
    }

    return std::monostate{};
  }

  [[nodiscard]] auto operator()(VariableExpression const& expr) -> Object {
    if (auto const found{resolution_.find(expr.name_)};
        found != resolution_.end()) {
      std::size_t const distance = found->second;
      return environment_->get_at(expr.name_.lexeme_, distance);
    } else {
      throw RuntimeError{expr.name_.line_,
                         expr.name_.lexeme_ + " is not defined"};
    }
  }

  [[nodiscard]] auto operator()(Box<AssignmentExpression> const& expr)
      -> Object {
    Object const value{std::visit(*this, expr->value_)};
    if (auto const found{resolution_.find(expr->name_)};
        found != resolution_.end()) {
      std::size_t const distance = found->second;
      environment_->assign_at(expr->name_.lexeme_, value, distance);
    } else {
      throw RuntimeError{expr->name_.line_,
                         expr->name_.lexeme_ + " is not defined"};
    }

    return value;
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
      return !is_equal(left, right);
    }
    if (op_type == TokenType::EQUAL_EQUAL) {
      return is_equal(left, right);
    }
    // Unreachable
    return std::monostate{};
  }

  [[nodiscard]] auto operator()(Box<CallExpression> const& expr) -> Object {
    Object callee{std::visit(*this, expr->callee_)};

    std::vector<Object> args{};
    for (auto const& arg : expr->arguments_) {
      args.push_back(std::visit(*this, arg));
    }

    try {
      std::size_t const arity{std::visit(Arity{}, callee)};
      if (args.size() != arity) {
        throw RuntimeError{expr->paren_.line_,
                           "Expected " + std::to_string(arity) +
                               " arguments but got " +
                               std::to_string(args.size()) + "."};
      }
      return std::visit(Call{environment_, resolution_, args}, callee);
    } catch (UncallableError const& e) {
      throw RuntimeError{expr->paren_.line_,
                         "Can only call functions and classes."};
    }
  }

  [[nodiscard]] auto operator()(Box<GroupingExpression> const& expr) -> Object {
    return std::visit(*this, expr->expression_);
  }

  [[nodiscard]] auto operator()(Box<LogicalExpression> const& expr) -> Object {
    Object const left{std::visit(*this, expr->left_)};

    if (expr->op_.type_ == TokenType::OR) {
      if (is_truthy(left)) {
        return left;
      }
    } else {
      if (!is_truthy(left)) {
        return left;
      }
    }
    return std::visit(*this, expr->right_);
  }

  [[nodiscard]] auto operator()(Box<UnaryExpression> const& expr) -> Object {
    Object const right{std::visit(*this, expr->right_)};

    Token const& op{expr->op_};
    TokenType const& op_type{op.type_};

    if (op_type == TokenType::MINUS) {
      check_number_operand(op, right);
      return -std::get<double>(right);
    }

    if (op_type == TokenType::BANG) {
      return !is_truthy(right);
    }

    return std::monostate{};
  }
};

struct StatementExecutor {
  std::shared_ptr<Environment> environment_;
  std::unordered_map<Token, std::size_t> const& resolution_;

  auto operator()(std::monostate) -> void {}

  auto operator()(ExpressionStatement const& stmt) -> void {
    static_cast<void>(std::visit(ExpressionEvaluator{environment_, resolution_},
                                 stmt.expression_));
  }

  auto operator()(PrintStatement const& stmt) -> void {
    Object const value{std::visit(
        ExpressionEvaluator{environment_, resolution_}, stmt.expression_)};
    std::visit(Put{std::cout}, value);
  }

  auto operator()(ReturnStatement const& stmt) -> void {
    Object const value{
        !std::holds_alternative<std::monostate>(stmt.value_)
            ? std::visit(ExpressionEvaluator{environment_, resolution_},
                         stmt.value_)
            : std::monostate{}};

    throw Return{value};
  }

  auto operator()(VariableStatement const& stmt) -> void {
    Object const value{
        !std::holds_alternative<std::monostate>(stmt.initializer_)
            ? std::visit(ExpressionEvaluator{environment_, resolution_},
                         stmt.initializer_)
            : std::monostate{}};

    environment_->define(stmt.name_.lexeme_, value);
  }

  auto operator()(Box<BlockStatement> const& stmt) -> void {
    // Create new environment with the current environment as its
    // enclosing environment
    auto const env{std::make_shared<Environment>(environment_)};

    // Execute statements in the block with the new environment
    for (Statement const& statement : stmt->statements_) {
      std::visit(StatementExecutor{env, resolution_}, statement);
    }
  }

  auto operator()(Box<FunctionStatement> const& stmt) -> void {
    LoxFunction f{*stmt, environment_};
    environment_->define(stmt->name_.lexeme_, Box{f});
  }

  auto operator()(Box<IfStatement> const& stmt) -> void {
    if (is_truthy(std::visit(ExpressionEvaluator{environment_, resolution_},
                             stmt->condition_))) {
      std::visit(*this, stmt->then_branch_);
    } else {
      std::visit(*this, stmt->else_branch_);
    }
  }

  auto operator()(Box<WhileStatement> const& stmt) -> void {
    while (is_truthy(std::visit(ExpressionEvaluator{environment_, resolution_},
                                stmt->condition_))) {
      std::visit(*this, stmt->body_);
    }
  }
};
}  // namespace

auto Interpreter::interpret(
    std::vector<Statement> const& statements,
    std::unordered_map<Token, std::size_t> const& resolution,
    std::shared_ptr<Environment> const& environment) -> void {
  for (auto const& stmt : statements) {
    std::visit(StatementExecutor{environment, resolution}, stmt);
  }
}

auto Interpreter::interpret(
    std::vector<Statement> const& statements,
    std::unordered_map<Token, std::size_t> const& resolution) -> void {
  auto const env{std::make_shared<Environment>()};
  interpret(statements, resolution, env);
}
