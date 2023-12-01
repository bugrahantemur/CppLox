#ifndef LOX_EXPRESSION
#define LOX_EXPRESSION

#include <string>
#include <variant>

#include "token.h"
#include "utils/box.h"

struct LiteralExpression {
  using Literal = std::variant<std::monostate, double, std::string>;
  Literal value_;
};

using Expression =
    std::variant<LiteralExpression, Box<struct BinaryExpression>,
                 Box<struct UnaryExpression>, Box<struct Grouping>>;

struct BinaryExpression {
  Expression left_;
  Token op_;
  Expression right_;
};

struct UnaryExpression {
  Token op_;
  Expression right_;
};

struct Grouping {
  Expression expression_;
};

#endif
