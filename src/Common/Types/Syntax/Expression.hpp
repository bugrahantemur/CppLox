#ifndef LOX_TREEWALK_TYPES_SYNTAX_EXPRESSION
#define LOX_TREEWALK_TYPES_SYNTAX_EXPRESSION

#include <variant>
#include <vector>

#include "../../../../submodules/RustyPtr/include/RustyPtr/Box.hpp"
#include "../../../Common/Types/Tokens/Token.hpp"

namespace LOX::Common::Types::Syntax::Expressions {

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
  LOX::Common::Types::Token name;
  Expression value;
};

struct BinaryExpr {
  Expression left;
  LOX::Common::Types::Token op;
  Expression right;
};

struct CallExpr {
  Expression callee;
  LOX::Common::Types::Token paren;
  std::vector<Expression> arguments;
};

struct GetExpr {
  LOX::Common::Types::Token name;
  Expression object;
};

struct GroupingExpr {
  Expression expression;
};

struct LiteralExpr {
  LOX::Common::Types::Token::Literal value;
};

struct LogicalExpr {
  Expression left;
  LOX::Common::Types::Token op;
  Expression right;
};

struct SetExpr {
  LOX::Common::Types::Token name;
  Expression object;
  Expression value;
};

struct SuperExpr {
  LOX::Common::Types::Token keyword;
  LOX::Common::Types::Token method;
};

struct ThisExpr {
  LOX::Common::Types::Token keyword;
};

struct UnaryExpr {
  LOX::Common::Types::Token op;
  Expression right;
};

struct VariableExpr {
  LOX::Common::Types::Token name;
};

}  // namespace LOX::Common::Types::Syntax::Expressions

#endif
