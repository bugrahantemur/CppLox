#ifndef LOX_EXPRESSION
#define LOX_EXPRESSION

#include <string>
#include <variant>

#include "token.h"
#include "utils/box.h"

struct LiteralExpression {
  using Literal = std::variant<std::monostate, bool, double, std::string>;
  Literal value_;
};

using Expression =
    std::variant<LiteralExpression, Box<struct BinaryExpression>,
                 Box<struct UnaryExpression>, Box<struct GroupingExpression>>;

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

#endif
