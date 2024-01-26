#ifndef LOX_TYPES_EXPRESSION
#define LOX_TYPES_EXPRESSION

#include <variant>

#include "../utils/box.hpp"
#include "./token.hpp"

namespace LOX {

// Forward declaration of all expression types
namespace Expressions {
struct AssignmentExpression;
struct BinaryExpression;
struct CallExpression;
struct GetExpression;
struct GroupingExpression;
struct LiteralExpression;
struct LogicalExpression;
struct SetExpression;
struct SuperExpression;
struct ThisExpression;
struct UnaryExpression;
struct VariableExpression;
}  // namespace Expressions

using Expression = std::variant<
    std::monostate, Box<Expressions::AssignmentExpression>,
    Box<Expressions::BinaryExpression>, Box<Expressions::CallExpression>,
    Box<Expressions::GetExpression>, Box<Expressions::GroupingExpression>,
    Box<Expressions::LiteralExpression>, Box<Expressions::LogicalExpression>,
    Box<Expressions::SetExpression>, Box<Expressions::SuperExpression>,
    Box<Expressions::ThisExpression>, Box<Expressions::UnaryExpression>,
    Box<Expressions::VariableExpression>>;

}  // namespace LOX
namespace LOX::Expressions {

struct AssignmentExpression {
  Token name_;
  Expression value_;
};

struct BinaryExpression {
  Expression left_;
  Token op_;
  Expression right_;
};

struct CallExpression {
  Expression callee_;
  Token paren_;
  std::vector<Expression> arguments_;
};

struct GetExpression {
  Token name_;
  Expression object_;
};

struct GroupingExpression {
  Expression expression_;
};

struct LiteralExpression {
  Token::Literal value_;
};

struct LogicalExpression {
  Expression left_;
  Token op_;
  Expression right_;
};

struct SetExpression {
  Token name_;
  Expression object_;
  Expression value_;
};

struct SuperExpression {
  Token keyword_;
  Token method_;
};

struct ThisExpression {
  Token keyword_;
};

struct UnaryExpression {
  Token op_;
  Expression right_;
};

struct VariableExpression {
  Token name_;
};

}  // namespace LOX::Expressions

#endif
