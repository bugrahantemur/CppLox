
#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER
#include <string>
#include <variant>

#include "expression.h"
#include "token.h"
#include "utils/box.h"
namespace {

using Object = std::variant<std::monostate, bool, double, std::string>;
struct Interpreter {
  auto operator()(LiteralExpression const& expr) -> Object {
    return expr.value_;
  }

  auto operator()(Box<GroupingExpression> const& expr) -> Object {
    return std::visit(*this, expr->expression_);
  }

  auto operator()(Box<UnaryExpression> const& expr) -> Object {
    Object const right = std::visit(*this, expr->right_);

    TokenType const op = expr->op_.type_;

    if (op == TokenType::MINUS) {
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

    if (op == TokenType::BANG) {
      return !is_truthy(right);
    }

    return std::monostate{};
  }

  auto operator()(Box<BinaryExpression> const& expr) -> Object {
    Object const left = std::visit(*this, expr->left_);
    Object const right = std::visit(*this, expr->right_);

    TokenType const op = expr->op_.type_;

    if (op == TokenType::MINUS) {
      return std::get<double>(left) - std::get<double>(right);
    }
    if (op == TokenType::SLASH) {
      return std::get<double>(left) / std::get<double>(right);
    }
    if (op == TokenType::STAR) {
      return std::get<double>(left) * std::get<double>(right);
    }
    if (op == TokenType::PLUS) {
      if (std::holds_alternative<double>(left) &&
          std::holds_alternative<double>(right)) {
        return std::get<double>(left) + std::get<double>(right);
      }
      if (std::holds_alternative<std::string>(left) &&
          std::holds_alternative<std::string>(right)) {
        return std::get<std::string>(left) + std::get<std::string>(right);
      }
    }
    if (op == TokenType::GREATER) {
      return std::get<double>(left) > std::get<double>(right);
    }
    if (op == TokenType::GREATER_EQUAL) {
      return std::get<double>(left) >= std::get<double>(right);
    }
    if (op == TokenType::LESS) {
      return std::get<double>(left) < std::get<double>(right);
    }
    if (op == TokenType::LESS_EQUAL) {
      return std::get<double>(left) <= std::get<double>(right);
    }
    if (op == TokenType::BANG_EQUAL) {
      return left != right;
    }
    if (op == TokenType::EQUAL_EQUAL) {
      return left == right;
    }
    return std::monostate{};
  }
};
}  // namespace

#endif
