
#ifndef LOX_STATEMENT
#define LOX_STATEMENT

#include <variant>

#include "expression.h"

struct ExpressionStatement {
  Expression expression_;
};

struct PrintStatement {
  Expression expression_;
};

using Statement =
    std::variant<std::monostate, ExpressionStatement, PrintStatement>;

#endif
