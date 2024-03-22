
#ifndef LOX_TREEWALK_TYPES_SYNTAX_STATEMENT
#define LOX_TREEWALK_TYPES_SYNTAX_STATEMENT

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
using Statement =
    std::variant<std::monostate, Box<Statements::BlockStmt>,
                 Box<Statements::ClassStmt>, Box<Statements::ExpressionStmt>,
                 Box<Statements::FunctionStmt>, Box<Statements::IfStmt>,
                 Box<Statements::PrintStmt>, Box<Statements::ReturnStmt>,
                 Box<Statements::VariableStmt>, Box<Statements::WhileStmt>>;

struct BlockStmt {
  std::vector<Statement> statements;
};

struct ClassStmt {
  LOX::Common::Types::Token name;
  Expressions::VariableExpr super_class;
  std::vector<Box<FunctionStmt>> methods;
};

struct ExpressionStmt {
  Expressions::Expression expression;
};

struct FunctionStmt {
  LOX::Common::Types::Token name;
  std::vector<LOX::Common::Types::Token> params;
  std::vector<Statement> body;
};

struct IfStmt {
  Expressions::Expression condition;
  Statement then_branch;
  Statement else_branch;
};

struct PrintStmt {
  Expressions::Expression expression;
};

struct ReturnStmt {
  LOX::Common::Types::Token keyword;
  Expressions::Expression value;
};

struct VariableStmt {
  LOX::Common::Types::Token name;
  Expressions::Expression initializer;
};

struct WhileStmt {
  Expressions::Expression condition;
  Statement body;
};

}  // namespace LOX::Common::Types::Syntax::Statements

#endif
