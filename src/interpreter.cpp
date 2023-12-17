
#include <string>
#include <variant>

#include "expression.h"
#include "token.h"
#include "utils/box.h"
#include "utils/error.h"

namespace {

using Object = std::variant<std::monostate, bool, double, std::string>;

template <typename... Objects>
auto check_number_operand(Token const& token, Objects... operands) -> void {
  std::array const ops{operands...};
  if (std::all_of(std::begin(ops), std::end(ops), [](Object const& obj) {
        return !std::holds_alternative<double>(obj);
      })) {
    throw RuntimeError{token, sizeof...(operands) > 1
                                  ? "Operands must be numbers."
                                  : "Operand must be a number."};
  }
}

struct ExpressionVisitor {
  auto operator()(std::monostate const& none) -> Object {
    return std::monostate{};
  }
  auto operator()(LiteralExpression const& expr) -> Object {
    return expr.value_;
  }

  auto operator()(Box<GroupingExpression> const& expr) -> Object {
    return std::visit(*this, expr->expression_);
  }

  auto operator()(Box<UnaryExpression> const& expr) -> Object {
    Object const right = std::visit(*this, expr->right_);

    Token const& op = expr->op_;
    TokenType const& op_type = op.type_;

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

  auto operator()(Box<BinaryExpression> const& expr) -> Object {
    Object const left = std::visit(*this, expr->left_);
    Object const right = std::visit(*this, expr->right_);

    Token const& op = expr->op_;
    TokenType const& op_type = op.type_;

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
      throw RuntimeError{op, "Operands must be two numbers or two strings."};
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
};

auto stringify(Object const& obj) -> std::string {
  if (std::holds_alternative<std::string>(obj)) {
    return std::get<std::string>(obj);
  }
  if (std::holds_alternative<double>(obj)) {
    auto const str = std::to_string(std::get<double>(obj));
    if (std::size_t const found = str.find('.'); found != std::string::npos) {
      return str.substr(0, found);
    }
  }
  if (std::holds_alternative<bool>(obj)) {
    return std::get<bool>(obj) ? "true" : "false";
  }
  if (std::holds_alternative<std::monostate>(obj)) {
    return "nil";
  }
  // Unreachable
  return "";
}

template <typename OStream>
struct Put {
  Put(OStream& out) : out_{out} {}

  template <typename T>
  auto put(T const& obj) -> OStream& {
    out_ << obj << '\n';
    return out_;
  }

  auto operator()(std::string const& str) -> OStream& { return put(str); }

  auto operator()(double const number) -> OStream& { return put(number); }

  auto operator()(bool const b) -> OStream& {
    return b ? put("true") : put("false");
  }

  auto operator()(std::monostate) -> OStream& { return put("nil"); }

  OStream& out_;
};
}  // namespace

namespace Interpreter {
auto interpret(Expression const& expr) -> void {
  try {
    Object const obj = std::visit(ExpressionVisitor{}, expr);
    std::visit(Put{std::cout}, obj);
  } catch (RuntimeError const& e) {
    report(e);
    throw e;
  }
}
}  // namespace Interpreter
