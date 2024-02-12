#ifndef LOX_TYPES_SYNTAX_EXPRESSION
#define LOX_TYPES_SYNTAX_EXPRESSION

#include <variant>

#include "../../../RustyPtr/include/RustyPtr/Box.hpp"
#include "../../Types/Tokens/Token.hpp"

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
  Tokens::Token name;
  Expression value;
};

struct BinaryExpr {
  Expression left;
  Tokens::Token op;
  Expression right;
};

struct CallExpr {
  Expression callee;
  Tokens::Token paren;
  std::vector<Expression> arguments;
};

struct GetExpr {
  Tokens::Token name;
  Expression object;
};

struct GroupingExpr {
  Expression expression;
};

struct LiteralExpr {
  Tokens::Token::Literal value;
};

struct LogicalExpr {
  Expression left;
  Tokens::Token op;
  Expression right;
};

struct SetExpr {
  Tokens::Token name;
  Expression object;
  Expression value;
};

struct SuperExpr {
  Tokens::Token keyword;
  Tokens::Token method;
};

struct ThisExpr {
  Tokens::Token keyword;
};

struct UnaryExpr {
  Tokens::Token op;
  Expression right;
};

struct VariableExpr {
  Tokens::Token name;
};

}  // namespace LOX::Types::Syntax::Expressions

#endif
