#ifndef LOX_INTERPRETER_EXPRESSION_EVALUATOR
#define LOX_INTERPRETER_EXPRESSION_EVALUATOR

#include <optional>
#include <unordered_map>

#include "../../Types/Object/Class.hpp"
#include "../../Types/Object/Function.hpp"
#include "../../Types/Object/Object.hpp"
#include "../../Types/Syntax/Expression.hpp"
#include "../../Types/Token/Token.hpp"
#include "../../Utils/Arc.hpp"
#include "../../Utils/Box.hpp"
#include "../Environment/Environment.hpp"
#include "../Error/Error.hpp"
#include "../Utils/Operands/Operands.hpp"
#include "../Utils/Truth/Truth.hpp"
#include "./Call/Call.hpp"
#include "./Equality/Equality.hpp"
#include "./Instance/Instance.hpp"

namespace LOX::Interpreter::Expressions {

using namespace Utils;

using Types::Syntax::Expression;

using namespace Types::Objects;
using namespace Types::Syntax::Expressions;

struct ExpressionEvaluator {
  Arc<Environment> environment_;
  std::unordered_map<Token, std::size_t> const& resolution_;

  [[nodiscard]] auto operator()(std::monostate) -> Object {
    return std::monostate{};
  }

  [[nodiscard]] auto operator()(Box<LiteralExpr> const& expr) -> Object {
    return std::visit([](auto const& value) -> Object { return value; },
                      expr->value_);
  }

  [[nodiscard]] auto operator()(Box<SuperExpr> const& expr) -> Object {
    std::size_t const distance{resolution_.at(expr->keyword_)};

    auto const superclass{
        std::get<Box<LoxClass>>(environment_->get_at("super", distance))};

    auto const instance{
        std::get<Arc<LoxInstance>>(environment_->get_at("this", distance - 1))};

    std::optional<LoxFunction> const method{
        superclass->find_method(expr->method_.lexeme_)};

    if (!method) {
      throw Error{expr->method_.line_,
                  "Undefined property '" + expr->method_.lexeme_ + "'"};
    }

    auto env{Arc{Environment{method.value().closure_}}};
    env->define("this", instance);
    return Box{LoxFunction{method.value().declaration_, env,
                           method.value().is_initializer_}};
  }

  [[nodiscard]] auto operator()(Box<ThisExpr> const& expr) -> Object {
    return (*this)(VariableExpr{expr->keyword_});
  }

  [[nodiscard]] auto operator()(Box<VariableExpr> const& expr) -> Object {
    if (auto const found{resolution_.find(expr->name_)};
        found != resolution_.end()) {
      std::size_t const distance = found->second;
      return environment_->get_at(expr->name_.lexeme_, distance);
    } else {
      throw Error{expr->name_.line_, expr->name_.lexeme_ + " is not defined"};
    }
  }

  [[nodiscard]] auto operator()(Box<AssignmentExpr> const& expr) -> Object {
    Object const value{std::visit(*this, expr->value_)};
    if (auto const found{resolution_.find(expr->name_)};
        found != resolution_.end()) {
      std::size_t const distance{found->second};
      environment_->assign_at(expr->name_.lexeme_, value, distance);
    } else {
      throw Error{expr->name_.line_, expr->name_.lexeme_ + " is not defined"};
    }

    return value;
  }

  [[nodiscard]] auto operator()(Box<BinaryExpr> const& expr) -> Object {
    Object const left{std::visit(*this, expr->left_)};
    Object const right{std::visit(*this, expr->right_)};

    Token const& op{expr->op_};
    TokenType const& op_type{op.type_};

    if (op_type == TokenType::MINUS) {
      Operands::check_number_operand(op, left, right);
      return std::get<double>(left) - std::get<double>(right);
    }
    if (op_type == TokenType::SLASH) {
      Operands::check_number_operand(op, left, right);
      return std::get<double>(left) / std::get<double>(right);
    }
    if (op_type == TokenType::STAR) {
      Operands::check_number_operand(op, left, right);
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
      throw Error{op.line_, "Operands must be two numbers or two strings."};
    }
    if (op_type == TokenType::GREATER) {
      Operands::check_number_operand(op, left, right);
      return std::get<double>(left) > std::get<double>(right);
    }
    if (op_type == TokenType::GREATER_EQUAL) {
      Operands::check_number_operand(op, left, right);
      return std::get<double>(left) >= std::get<double>(right);
    }
    if (op_type == TokenType::LESS) {
      Operands::check_number_operand(op, left, right);
      return std::get<double>(left) < std::get<double>(right);
    }
    if (op_type == TokenType::LESS_EQUAL) {
      Operands::check_number_operand(op, left, right);
      return std::get<double>(left) <= std::get<double>(right);
    }
    try {
      if (op_type == TokenType::BANG_EQUAL) {
        return !is_equal(left, right);
      }
      if (op_type == TokenType::EQUAL_EQUAL) {
        return is_equal(left, right);
      }
    } catch (NotComparableError const& e) {
      throw Error{
          op.line_,
          "Can only compare booleans, strings, and numbers for equality."};
    }
    // Unreachable
    return std::monostate{};
  }

  [[nodiscard]] auto operator()(Box<CallExpr> const& expr) -> Object {
    Object callee{std::visit(*this, expr->callee_)};

    std::vector<Object> args{};
    for (auto const& arg : expr->arguments_) {
      args.push_back(std::visit(*this, arg));
    }

    try {
      std::size_t const arity{arity_of(callee)};
      if (args.size() != arity) {
        throw Error{expr->paren_.line_, "Expected " + std::to_string(arity) +
                                            " arguments but got " +
                                            std::to_string(args.size()) + "."};
      }
      return call(environment_, resolution_, args, callee);
    } catch (UncallableError const& e) {
      throw Error{expr->paren_.line_, "Can only call functions and classes."};
    }
  }

  [[nodiscard]] auto operator()(Box<GetExpr> const& expr) -> Object {
    Object const obj{std::visit(*this, expr->object_)};

    if (Arc<LoxInstance> const* const instance{
            std::get_if<Arc<LoxInstance>>(&obj)}) {
      return Instance::get(*instance, expr->name_);
    }

    throw Error{expr->name_.line_, "Only instances have properties."};
  }

  [[nodiscard]] auto operator()(Box<GroupingExpr> const& expr) -> Object {
    return std::visit(*this, expr->expression_);
  }

  [[nodiscard]] auto operator()(Box<LogicalExpr> const& expr) -> Object {
    Object const left{std::visit(*this, expr->left_)};

    if (expr->op_.type_ == TokenType::OR) {
      if (Utils::is_truthy(left)) {
        return left;
      }
    } else {
      if (!Utils::is_truthy(left)) {
        return left;
      }
    }
    return std::visit(*this, expr->right_);
  }

  [[nodiscard]] auto operator()(Box<SetExpr> const& expr) -> Object {
    Object obj{std::visit(*this, expr->object_)};

    if (auto const instance{std::get_if<Arc<LoxInstance>>(&obj)}) {
      Object const value{std::visit(*this, expr->value_)};
      Instance::set(*instance, expr->name_, value);

      return value;
    }

    throw Error{expr->name_.line_, "Only instances have properties."};
  }
  [[nodiscard]] auto operator()(Box<UnaryExpr> const& expr) -> Object {
    Object const right{std::visit(*this, expr->right_)};

    Token const& op{expr->op_};
    TokenType const& op_type{op.type_};

    if (op_type == TokenType::MINUS) {
      Utils::Operands::check_number_operand(op, right);
      return -std::get<double>(right);
    }

    if (op_type == TokenType::BANG) {
      return !is_truthy(right);
    }

    return std::monostate{};
  }
};
}  // namespace LOX::Interpreter::Expressions
#endif
