
#ifndef LOX_TYPES_STATEMENT
#define LOX_TYPES_STATEMENT

#include <string>
#include <variant>

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

using Statement = std::variant<std::monostate, ExpressionStatement,
                               PrintStatement, VariableStatement>;

#endif