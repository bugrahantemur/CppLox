
#ifndef LOX_TREEWALK_TYPES_SYNTAX_STATEMENT
#define LOX_TREEWALK_TYPES_SYNTAX_STATEMENT

#include <cstddef>
#include <variant>

#include "../../../../submodules/RustyPtr/include/RustyPtr/Box.hpp"
#include "../../../Common/Types/Tokens/Token.hpp"
#include "./Expression.hpp"

namespace LOX::Common::Types::Syntax::Statements {

// Forward declaration of all statement types
struct BlockStmt;
struct ClassStmt;
struct ExpressionStmt;
struct FunctionStmt;
struct IfStmt;
struct PrintStmt;
struct ReturnStmt;
struct VariableStmt;
struct WhileStmt;

// Statement variant
using Statement = std::variant<std::monostate, Box<BlockStmt>, Box<ClassStmt>,
                               Box<ExpressionStmt>, Box<FunctionStmt>,
                               Box<IfStmt>, Box<PrintStmt>, Box<ReturnStmt>,
                               Box<VariableStmt>, Box<WhileStmt>>;

struct BlockStmt {
  Token beginning;
  std::vector<Statement> statements;
};

struct ClassStmt {
  Token name;
  Expressions::VariableExpr super_class;
  std::vector<Box<FunctionStmt>> methods;
};

struct ExpressionStmt {
  Token ending;
  Expressions::Expression expression;
};

struct FunctionStmt {
  Token name;
  std::vector<Token> params;
  std::vector<Statement> body;
};

struct IfStmt {
  Token keyword;
  Expressions::Expression condition;
  Statement then_branch;
  Statement else_branch;
};

struct PrintStmt {
  Token keyword;
  Expressions::Expression expression;
};

struct ReturnStmt {
  Token keyword;
  Expressions::Expression value;
};

struct VariableStmt {
  Token name;
  Expressions::Expression initializer;
};

struct WhileStmt {
  Token keyword;
  Expressions::Expression condition;
  Statement body;
};

}  // namespace LOX::Common::Types::Syntax::Statements

#endif
