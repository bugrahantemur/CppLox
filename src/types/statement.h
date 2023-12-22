
#ifndef LOX_TYPES_STATEMENT
#define LOX_TYPES_STATEMENT

#include <string>
#include <variant>

#include "../utils/box.h"
#include "./expression.h"

struct ExpressionStatement {
  Expression expression_;
};

struct PrintStatement {
  Expression expression_;
};

struct VariableStatement {
  std::string name_;
  Expression initializer_;
};

using Statement =
    std::variant<std::monostate, ExpressionStatement, PrintStatement,
                 VariableStatement, Box<struct BlockStatement>,
                 Box<struct IfStatement>>;

struct BlockStatement {
  std::vector<Statement> statements_;
};

struct IfStatement {
  Expression condition_;
  Statement then_branch_;
  Statement else_branch_;
};

#endif
