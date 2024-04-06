#ifndef LOX_TREEWALK_TYPES_SYNTAX_EXPRESSION
#define LOX_TREEWALK_TYPES_SYNTAX_EXPRESSION

#include <variant>
#include <vector>

#include "../../../../submodules/RustyPtr/include/RustyPtr/Box.hpp"
#include "../../../Common/Types/Tokens/Token.hpp"
#include "../../../Common/Types/Value/Value.hpp"

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
    std::variant<std::monostate, Box<AssignmentExpr>, Box<BinaryExpr>,
                 Box<CallExpr>, Box<GetExpr>, Box<GroupingExpr>,
                 Box<LiteralExpr>, Box<LogicalExpr>, Box<SetExpr>,
                 Box<SuperExpr>, Box<ThisExpr>, Box<UnaryExpr>,
                 Box<VariableExpr>>;

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
  std::size_t line;
  Value value;
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

}  // namespace LOX::Common::Types::Syntax::Expressions

#endif
