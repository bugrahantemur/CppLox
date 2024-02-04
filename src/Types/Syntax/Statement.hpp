
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
  std::vector<Statement> statements;
};

struct ClassStmt {
  Token name;
  Expressions::VariableExpr super_class;
  std::vector<Box<FunctionStmt>> methods;
};

struct ExpressionStmt {
  Expression expression;
};

struct FunctionStmt {
  Token name;
  std::vector<Token> params;
  std::vector<Statement> body;
};

struct IfStmt {
  Expression condition;
  Statement then_branch;
  Statement else_branch;
};

struct PrintStmt {
  Expression expression;
};

struct ReturnStmt {
  Token keyword;
  Expression value;
};

struct VariableStmt {
  Token name;
  Expression initializer;
};

struct WhileStmt {
  Expression condition;
  Statement body;
};

}  // namespace LOX::Types::Syntax::Statements

#endif
