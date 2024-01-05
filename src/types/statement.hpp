
#ifndef LOX_TYPES_STATEMENT
#define LOX_TYPES_STATEMENT

#include <string>
#include <variant>

#include "../utils/box.hpp"
#include "./expression.hpp"
#include "./token.hpp"

struct ExpressionStatement {
  Expression expression_;
};

struct PrintStatement {
  Expression expression_;
};

struct ReturnStatement {
  Token keyword_;
  Expression value_;
};

struct VariableStatement {
  Token name_;
  Expression initializer_;
};

using Statement =
    std::variant<std::monostate, ExpressionStatement, PrintStatement,
                 ReturnStatement, VariableStatement, Box<struct BlockStatement>,
                 Box<struct FunctionStatement>, Box<struct ClassStatement>,
                 Box<struct IfStatement>, Box<struct WhileStatement>>;

struct BlockStatement {
  std::vector<Statement> statements_;
};

struct FunctionStatement {
  Token name_;
  std::vector<Token> params_;
  std::vector<Statement> body_;
};

struct ClassStatement {
  Token name_;
  std::vector<Box<FunctionStatement>> methods_;
};

struct IfStatement {
  Expression condition_;
  Statement then_branch_;
  Statement else_branch_;
};

struct WhileStatement {
  Expression condition_;
  Statement body_;
};

#endif
