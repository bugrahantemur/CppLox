
#ifndef LOX_STATEMENT
#define LOX_STATEMENT

#include <string>
#include <variant>

#include "expression.h"

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

using Statement = std::variant<std::monostate, ExpressionStatement,
                               PrintStatement, VariableStatement>;

#endif
