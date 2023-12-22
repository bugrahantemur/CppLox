#ifndef LOX_TYPES_EXPRESSION
#define LOX_TYPES_EXPRESSION

#include <string>
#include <variant>

#include "../utils/box.h"
#include "./token.h"

struct LiteralExpression {
  Token::Literal value_;
};

struct VariableExpression {
  Token name_;
};

using Expression =
    std::variant<std::monostate, LiteralExpression, VariableExpression,
                 Box<struct BinaryExpression>, Box<struct UnaryExpression>,
                 Box<struct GroupingExpression>,
                 Box<struct AssignmentExpression>,
                 Box<struct LogicalExpression>>;

struct BinaryExpression {
  Expression left_;
  Token op_;
  Expression right_;
};

struct UnaryExpression {
  Token op_;
  Expression right_;
};

struct GroupingExpression {
  Expression expression_;
};

struct AssignmentExpression {
  Token name_;
  Expression value_;
};

struct LogicalExpression {
  Expression left_;
  Token op_;
  Expression right_;
};

#endif
