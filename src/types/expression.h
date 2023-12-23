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
                 Box<struct AssignmentExpression>, Box<struct BinaryExpression>,
                 Box<struct CallExpression>, Box<struct GroupingExpression>,
                 Box<struct LogicalExpression>, Box<struct UnaryExpression>>;

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

struct GroupingExpression {
  Expression expression_;
};

struct LogicalExpression {
  Expression left_;
  Token op_;
  Expression right_;
};

struct UnaryExpression {
  Token op_;
  Expression right_;
};

#endif
