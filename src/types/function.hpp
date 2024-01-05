#ifndef LOX_TYPES_FUNCTION
#define LOX_TYPES_FUNCTION

#include <memory>

#include "../environment.hpp"
#include "./statement.hpp"

struct LoxFunction {
  FunctionStatement declaration_;
  std::shared_ptr<Environment> closure_;
};

#endif
