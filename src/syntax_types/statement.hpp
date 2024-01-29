
#ifndef LOX_TYPES_STATEMENT
#define LOX_TYPES_STATEMENT

#include <variant>

#include "../token/token.hpp"
#include "../utils/box.hpp"
#include "./expression.hpp"

namespace LOX {
// Forward declaration of all statement types

namespace Statements {
struct BlockStatement;
struct ClassStatement;
struct ExpressionStatement;
struct FunctionStatement;
struct IfStatement;
struct PrintStatement;
struct ReturnStatement;
struct VariableStatement;
struct WhileStatement;
}  // namespace Statements

// Statement variant
using Statement = std::variant<
    std::monostate, Box<Statements::BlockStatement>,
    Box<Statements::ClassStatement>, Box<Statements::ExpressionStatement>,
    Box<Statements::FunctionStatement>, Box<Statements::IfStatement>,
    Box<Statements::PrintStatement>, Box<Statements::ReturnStatement>,
    Box<Statements::VariableStatement>, Box<Statements::WhileStatement>>;

}  // namespace LOX
namespace LOX::Statements {

using namespace LOX::Expressions;

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

}  // namespace LOX::Statements

#endif
