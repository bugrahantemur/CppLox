#ifndef LOX_TYPES_EXPRESSION
#define LOX_TYPES_EXPRESSION

#include <string>
#include <variant>

#include "../utils/box.hpp"
#include "./token.hpp"

struct LiteralExpression {
  Token::Literal value_;
};

struct ThisExpression {
  Token keyword_;
};

struct VariableExpression {
  Token name_;
};

using Expression =
    std::variant<std::monostate, LiteralExpression, ThisExpression,
                 VariableExpression, Box<struct AssignmentExpression>,
                 Box<struct BinaryExpression>, Box<struct CallExpression>,
                 Box<struct GetExpression>, Box<struct GroupingExpression>,
                 Box<struct LogicalExpression>, Box<struct SetExpression>,
                 Box<struct UnaryExpression>>;

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

struct UnaryExpression {
  Token op_;
  Expression right_;
};

#endif
