
#ifndef LOX_TYPES_STATEMENT
#define LOX_TYPES_STATEMENT

#include <variant>

#include "../../Types/Token/Token.hpp"
#include "../../Utils/Box.hpp"
#include "./Expression.hpp"

namespace LOX::Types::Syntax {
// Forward declaration of all statement types

namespace Statements {
struct BlockStmt;
struct ClassStmt;
struct ExpressionStmt;
struct FunctionStmt;
struct IfStmt;
struct PrintStmt;
struct ReturnStmt;
struct VariableStmt;
struct WhileStmt;
}  // namespace Statements

// Statement variant
using Statement =
    std::variant<std::monostate, Box<Statements::BlockStmt>,
                 Box<Statements::ClassStmt>, Box<Statements::ExpressionStmt>,
                 Box<Statements::FunctionStmt>, Box<Statements::IfStmt>,
                 Box<Statements::PrintStmt>, Box<Statements::ReturnStmt>,
                 Box<Statements::VariableStmt>, Box<Statements::WhileStmt>>;

}  // namespace LOX::Types::Syntax

namespace LOX::Types::Syntax::Statements {

struct BlockStmt {
  std::vector<Statement> statements_;
};

struct ClassStmt {
  Token name_;
  Expressions::VariableExpr super_class_;
  std::vector<Box<FunctionStmt>> methods_;
};

struct ExpressionStmt {
  Expression expression_;
};

struct FunctionStmt {
  Token name_;
  std::vector<Token> params_;
  std::vector<Statement> body_;
};

struct IfStmt {
  Expression condition_;
  Statement then_branch_;
  Statement else_branch_;
};

struct PrintStmt {
  Expression expression_;
};

struct ReturnStmt {
  Token keyword_;
  Expression value_;
};

struct VariableStmt {
  Token name_;
  Expression initializer_;
};

struct WhileStmt {
  Expression condition_;
  Statement body_;
};

}  // namespace LOX::Types::Syntax::Statements

#endif
