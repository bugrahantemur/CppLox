
#ifndef LOX_TYPES_STATEMENT
#define LOX_TYPES_STATEMENT

#include <variant>

#include "../utils/box.hpp"
#include "./expression.hpp"
#include "./token.hpp"

namespace LOX {

// Forward declaration of all statement types
struct BlockStatement;
struct ClassStatement;
struct ExpressionStatement;
struct FunctionStatement;
struct IfStatement;
struct PrintStatement;
struct ReturnStatement;
struct VariableStatement;
struct WhileStatement;

// Statement variant
using Statement =
    std::variant<std::monostate, Box<BlockStatement>, Box<ClassStatement>,
                 Box<ExpressionStatement>, Box<FunctionStatement>,
                 Box<IfStatement>, Box<PrintStatement>, Box<ReturnStatement>,
                 Box<VariableStatement>, Box<WhileStatement>>;

struct BlockStatement {
  std::vector<Statement> statements_;
};

struct ClassStatement {
  Token name_;
  VariableExpression super_class_;
  std::vector<Box<FunctionStatement>> methods_;
};

struct ExpressionStatement {
  Expression expression_;
};

struct FunctionStatement {
  Token name_;
  std::vector<Token> params_;
  std::vector<Statement> body_;
};

struct IfStatement {
  Expression condition_;
  Statement then_branch_;
  Statement else_branch_;
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

struct WhileStatement {
  Expression condition_;
  Statement body_;
};

}  // namespace LOX

#endif
