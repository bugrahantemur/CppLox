#include <string>
#include <variant>

#include "token.h"
#include "utils/box.h"

struct LiteralExpression {
  using Literal = std::variant<std::monostate, double, std::string>;

  template <typename T>
  LiteralExpression(T const& value);

  Literal value_;
};

using Expression =
    std::variant<LiteralExpression, Box<struct BinaryExpression>,
                 Box<struct UnaryExpression>, Box<struct Grouping>>;

struct BinaryExpression {
  BinaryExpression(Expression const& left, Token const& op,
                   Expression const& right);

  Expression left_;
  Token op_;
  Expression right_;
};

struct UnaryExpression {
  UnaryExpression(Token const& op, Expression const& right);
  Token op_;
  Expression right_;
};

struct Grouping {
  Grouping(Expression const& expression);
  Expression expression_;
};

std::string print_ast(Expression const& expression);
