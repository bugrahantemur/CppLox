#ifndef LOX_TYPES_SYNTAX_EXPRESSION
#define LOX_TYPES_SYNTAX_EXPRESSION

#include <variant>

#include "../../Types/Token/Token.hpp"
#include "../../Utils/Box.hpp"

namespace LOX::Types::Syntax::Expressions {

// Forward declaration of all expression types
struct AssignmentExpr;
struct BinaryExpr;
struct CallExpr;
struct GetExpr;
struct GroupingExpr;
struct LiteralExpr;
struct LogicalExpr;
struct SetExpr;
struct SuperExpr;
struct ThisExpr;
struct UnaryExpr;
struct VariableExpr;

using Expression =
    std::variant<std::monostate, Box<Expressions::AssignmentExpr>,
                 Box<Expressions::BinaryExpr>, Box<Expressions::CallExpr>,
                 Box<Expressions::GetExpr>, Box<Expressions::GroupingExpr>,
                 Box<Expressions::LiteralExpr>, Box<Expressions::LogicalExpr>,
                 Box<Expressions::SetExpr>, Box<Expressions::SuperExpr>,
                 Box<Expressions::ThisExpr>, Box<Expressions::UnaryExpr>,
                 Box<Expressions::VariableExpr>>;

struct AssignmentExpr {
  Token name;
  Expression value;
};

struct BinaryExpr {
  Expression left;
  Token op;
  Expression right;
};

struct CallExpr {
  Expression callee;
  Token paren;
  std::vector<Expression> arguments;
};

struct GetExpr {
  Token name;
  Expression object;
};

struct GroupingExpr {
  Expression expression;
};

struct LiteralExpr {
  Token::Literal value;
};

struct LogicalExpr {
  Expression left;
  Token op;
  Expression right;
};

struct SetExpr {
  Token name;
  Expression object;
  Expression value;
};

struct SuperExpr {
  Token keyword;
  Token method;
};

struct ThisExpr {
  Token keyword;
};

struct UnaryExpr {
  Token op;
  Expression right;
};

struct VariableExpr {
  Token name;
};

}  // namespace LOX::Types::Syntax::Expressions

#endif
