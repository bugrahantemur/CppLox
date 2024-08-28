#include "./Evaluate.hpp"

#include <optional>
#include <unordered_map>

#include "../../../../submodules/RustyPtr/include/RustyPtr/Arc.hpp"
#include "../../../../submodules/RustyPtr/include/RustyPtr/Box.hpp"
#include "../../../Common/Types/Syntax/Expression.hpp"
#include "../../../Common/Types/Tokens/Token.hpp"
#include "../../../Common/Types/Tokens/TokenTypes.hpp"
#include "../../../Common/Utils/Operands/Operands.hpp"
#include "../../../Common/Utils/Truth/Truth.hpp"
#include "../../Types/Environment/Environment.hpp"
#include "../../Types/Objects/Object.hpp"
#include "../Error/Error.hpp"
#include "./Call/Call.hpp"
#include "./Class/Class.hpp"
#include "./Equality/Equality.hpp"

namespace LOX::TreeWalk::Interpreter::Expressions {

using namespace LOX::Common::Types::Tokens;
using LOX::Common::Types::Token;
using namespace LOX::Common::Types::Syntax::Expressions;

using namespace LOX::TreeWalk::Types::Objects;
using LOX::TreeWalk::Types::Environment;

struct ExpressionEvaluator {
  Arc<Environment> environment;
  std::unordered_map<Token, std::size_t> const& resolution;

  [[nodiscard]] auto operator()(std::monostate none) -> Object { return none; }

  [[nodiscard]] auto operator()(Box<LiteralExpr> const& expr) -> Object {
    return std::visit([](auto const& value) -> Object { return value; },
                      expr->value);
  }

  [[nodiscard]] auto operator()(Box<SuperExpr> const& expr) -> Object {
    std::size_t const distance{resolution.at(expr->keyword)};

    auto const super_class{
        std::get<Box<LoxClass>>(environment->get_at("super", distance))};

    auto const instance{
        std::get<Arc<LoxInstance>>(environment->get_at("this", distance - 1))};

    std::optional<LoxFunction> const method{
        Class::find_method(*super_class, expr->method.lexeme)};

    if (!method) {
      throw Error{expr->method.line,
                  "Undefined property '" + expr->method.lexeme + "'"};
    }

    auto env{Arc{Environment{method.value().closure}}};
    env->define("this", instance);
    return Box{LoxFunction{method.value().declaration, env,
                           method.value().is_initializer}};
  }

  [[nodiscard]] auto operator()(Box<ThisExpr> const& expr) -> Object {
    return (*this)(VariableExpr{expr->keyword});
  }

  [[nodiscard]] auto operator()(Box<VariableExpr> const& expr) -> Object {
    if (auto const found{resolution.find(expr->name)};
        found != resolution.end()) {
      std::size_t const distance = found->second;
      return environment->get_at(expr->name.lexeme, distance);
    }

    throw Error{expr->name.line, expr->name.lexeme + " is not defined"};
  }

  [[nodiscard]] auto operator()(Box<AssignmentExpr> const& expr) -> Object {
    Object const value{std::visit(*this, expr->value)};
    if (auto const found{resolution.find(expr->name)};
        found != resolution.end()) {
      std::size_t const distance{found->second};
      environment->assign_at(expr->name.lexeme, value, distance);
    } else {
      throw Error{expr->name.line, expr->name.lexeme + " is not defined"};
    }

    return value;
  }

  [[nodiscard]] auto operator()(Box<BinaryExpr> const& expr) -> Object {
    Object const left{std::visit(*this, expr->left)};
    Object const right{std::visit(*this, expr->right)};

    Token const& op{expr->op};
    TokenType const& op_type{op.type};

    if (op_type == TokenType::MINUS) {
      Common::Utils::Operands::check_number_operand<Error>(op.line, left,
                                                           right);
      return std::get<double>(left) - std::get<double>(right);
    }
    if (op_type == TokenType::SLASH) {
      Common::Utils::Operands::check_number_operand<Error>(op.line, left,
                                                           right);
      return std::get<double>(left) / std::get<double>(right);
    }
    if (op_type == TokenType::STAR) {
      Common::Utils::Operands::check_number_operand<Error>(op.line, left,
                                                           right);
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
      throw Error{op.line, "Operands must be two numbers or two strings."};
    }
    if (op_type == TokenType::GREATER) {
      Common::Utils::Operands::check_number_operand<Error>(op.line, left,
                                                           right);
      return std::get<double>(left) > std::get<double>(right);
    }
    if (op_type == TokenType::GREATER_EQUAL) {
      Common::Utils::Operands::check_number_operand<Error>(op.line, left,
                                                           right);
      return std::get<double>(left) >= std::get<double>(right);
    }
    if (op_type == TokenType::LESS) {
      Common::Utils::Operands::check_number_operand<Error>(op.line, left,
                                                           right);
      return std::get<double>(left) < std::get<double>(right);
    }
    if (op_type == TokenType::LESS_EQUAL) {
      Common::Utils::Operands::check_number_operand<Error>(op.line, left,
                                                           right);
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
          op.line,
          "Can only compare booleans, strings, and numbers for equality."};
    }
    // Unreachable
    return std::monostate{};
  }

  [[nodiscard]] auto operator()(Box<CallExpr> const& expr) -> Object {
    Object callee{std::visit(*this, expr->callee)};

    std::vector<Object> args{};
    for (auto const& arg : expr->arguments) {
      args.push_back(std::visit(*this, arg));
    }

    try {
      std::size_t const arity{arity_of(callee)};
      if (args.size() != arity) {
        throw Error{expr->paren.line, "Expected " + std::to_string(arity) +
                                          " arguments but got " +
                                          std::to_string(args.size()) + "."};
      }
      return call(environment, resolution, args, callee);
    } catch (UncallableError const& e) {
      throw Error{expr->paren.line, "Can only call functions and classes."};
    }
  }

  [[nodiscard]] auto operator()(Box<GetExpr> const& expr) -> Object {
    Object const obj{std::visit(*this, expr->object)};

    if (Arc<LoxInstance> const* const instance{
            std::get_if<Arc<LoxInstance>>(&obj)}) {
      return Class::get(*instance, expr->name);
    }

    throw Error{expr->name.line, "Only instances have properties."};
  }

  [[nodiscard]] auto operator()(Box<GroupingExpr> const& expr) -> Object {
    return std::visit(*this, expr->expression);
  }

  [[nodiscard]] auto operator()(Box<LogicalExpr> const& expr) -> Object {
    Object const left{std::visit(*this, expr->left)};

    if (expr->op.type == TokenType::OR) {
      if (Common::Utils::is_truthy(left)) {
        return left;
      }
    } else {
      if (!Common::Utils::is_truthy(left)) {
        return left;
      }
    }
    return std::visit(*this, expr->right);
  }

  [[nodiscard]] auto operator()(Box<SetExpr> const& expr) -> Object {
    Object obj{std::visit(*this, expr->object)};

    if (auto* const instance{std::get_if<Arc<LoxInstance>>(&obj)}) {
      Object const value{std::visit(*this, expr->value)};
      Class::set(*instance, expr->name, value);

      return value;
    }

    throw Error{expr->name.line, "Only instances have properties."};
  }
  [[nodiscard]] auto operator()(Box<UnaryExpr> const& expr) -> Object {
    Object const right{std::visit(*this, expr->right)};

    Token const& op{expr->op};
    TokenType const& op_type{op.type};

    if (op_type == TokenType::MINUS) {
      Common::Utils::Operands::check_number_operand<Error>(op.line, right);
      return -std::get<double>(right);
    }

    if (op_type == TokenType::BANG) {
      return !Common::Utils::is_truthy(right);
    }

    return std::monostate{};
  }
};

auto evaluate(Expression const& expression, Arc<Environment> environment,
              std::unordered_map<Token, std::size_t> const& resolution)
    -> Types::Objects::Object {
  return std::visit(ExpressionEvaluator{environment, resolution}, expression);
}

}  // namespace LOX::TreeWalk::Interpreter::Expressions
