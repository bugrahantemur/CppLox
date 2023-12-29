#ifndef LOX_TYPES_FUNCTION
#define LOX_TYPES_FUNCTION

#include "../environment.hpp"
#include "./statement.hpp"

struct LoxFunction {
  FunctionStatement declaration_;
  Environment closure_;
};

#endif
