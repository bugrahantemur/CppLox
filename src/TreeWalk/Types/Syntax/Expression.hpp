#ifndef LOX_TYPES_SYNTAX_EXPRESSION
#define LOX_TYPES_SYNTAX_EXPRESSION

#include <variant>
#include <vector>

#include "../../../../submodules/RustyPtr/include/RustyPtr/Box.hpp"
#include "../../../Common/Types/Tokens/Token.hpp"

namespace LOX::TreeWalk::Types::Syntax::Expressions {

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
  LOX::Types::Token name;
  Expression value;
};

struct BinaryExpr {
  Expression left;
  LOX::Types::Token op;
  Expression right;
};

struct CallExpr {
  Expression callee;
  LOX::Types::Token paren;
  std::vector<Expression> arguments;
};

struct GetExpr {
  LOX::Types::Token name;
  Expression object;
};

struct GroupingExpr {
  Expression expression;
};

struct LiteralExpr {
  LOX::Types::Token::Literal value;
};

struct LogicalExpr {
  Expression left;
  LOX::Types::Token op;
  Expression right;
};

struct SetExpr {
  LOX::Types::Token name;
  Expression object;
  Expression value;
};

struct SuperExpr {
  LOX::Types::Token keyword;
  LOX::Types::Token method;
};

struct ThisExpr {
  LOX::Types::Token keyword;
};

struct UnaryExpr {
  LOX::Types::Token op;
  Expression right;
};

struct VariableExpr {
  LOX::Types::Token name;
};

}  // namespace LOX::TreeWalk::Types::Syntax::Expressions

#endif
