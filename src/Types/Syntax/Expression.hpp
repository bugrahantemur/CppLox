#ifndef LOX_TYPES_EXPRESSION
#define LOX_TYPES_EXPRESSION

#include <variant>

#include "../../Types/Token/Token.hpp"
#include "../../Utils/Box.hpp"

namespace LOX::Types::Syntax {

// Forward declaration of all expression types
namespace Expressions {
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
}  // namespace Expressions

using Expression =
    std::variant<std::monostate, Box<Expressions::AssignmentExpr>,
                 Box<Expressions::BinaryExpr>, Box<Expressions::CallExpr>,
                 Box<Expressions::GetExpr>, Box<Expressions::GroupingExpr>,
                 Box<Expressions::LiteralExpr>, Box<Expressions::LogicalExpr>,
                 Box<Expressions::SetExpr>, Box<Expressions::SuperExpr>,
                 Box<Expressions::ThisExpr>, Box<Expressions::UnaryExpr>,
                 Box<Expressions::VariableExpr>>;

}  // namespace LOX::Types::Syntax

namespace LOX::Types::Syntax::Expressions {

struct AssignmentExpr {
  Token name_;
  Expression value_;
};

struct BinaryExpr {
  Expression left_;
  Token op_;
  Expression right_;
};

struct CallExpr {
  Expression callee_;
  Token paren_;
  std::vector<Expression> arguments_;
};

struct GetExpr {
  Token name_;
  Expression object_;
};

struct GroupingExpr {
  Expression expression_;
};

struct LiteralExpr {
  Token::Literal value_;
};

struct LogicalExpr {
  Expression left_;
  Token op_;
  Expression right_;
};

struct SetExpr {
  Token name_;
  Expression object_;
  Expression value_;
};

struct SuperExpr {
  Token keyword_;
  Token method_;
};

struct ThisExpr {
  Token keyword_;
};

struct UnaryExpr {
  Token op_;
  Expression right_;
};

struct VariableExpr {
  Token name_;
};

}  // namespace LOX::Types::Syntax::Expressions

#endif
