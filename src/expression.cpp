#include "expression.h"

#include <string>
#include <variant>

#include "utils/box.h"

template <typename T>
LiteralExpression::LiteralExpression(T const& value) : value_(value){};

BinaryExpression::BinaryExpression(Expression const& left, Token const& op,
                                   Expression const& right)
    : left_(left), op_(op), right_(right){};

UnaryExpression::UnaryExpression(Token const& op, Expression const& right)
    : op_(op), right_(right){};

Grouping::Grouping(Expression const& expression) : expression_(expression){};

template <typename T, typename... Args>
std::string paranthesize() {}

std::string print_ast(Expression const& expression) {
  struct AstPrinter {
    std::string operator()(
        Box<BinaryExpression> const& binary_expression) const {
      std::string result;

      result += "(";
      result += std::visit(*this, binary_expression->left_);
      result += " ";
      // result +=  magic_enum::enum_name(binary_expression->op_.type_);
      result += " ";
      result += std::visit(*this, binary_expression->right_);
      result += ")";

      return result;
    }
    std::string operator()(Box<UnaryExpression> const& unary_expression) const {
      std::string result;

      result += "(";
      result +=  // std::visit(*this, unary_expression->op_);
          result += " ";
      result += std::visit(*this, unary_expression->right_);
      result += ")";

      return result;
    }
    std::string operator()(Box<Grouping> const& grouping) const {
      std::string result;

      result += "(";
      result += std::visit(*this, grouping->expression_);
      result += ")";

      return result;
    }
    std::string operator()(LiteralExpression const& literal_expression) const {
      if (std::holds_alternative<std::monostate>(literal_expression.value_)) {
        return "nil";
      } else if (std::holds_alternative<double>(literal_expression.value_)) {
        return std::to_string(std::get<double>(literal_expression.value_));
      } else if (std::holds_alternative<std::string>(
                     literal_expression.value_)) {
        return std::get<std::string>(literal_expression.value_);
      }

      return "";
    }
  };

  return std::visit(AstPrinter{}, expression);
}
