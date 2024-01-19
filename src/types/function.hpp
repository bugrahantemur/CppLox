#ifndef LOX_TYPES_FUNCTION
#define LOX_TYPES_FUNCTION

#include <memory>

#include "./environment.hpp"
#include "./statement.hpp"

namespace LOX {

struct LoxFunction {
  FunctionStatement declaration_;
  std::shared_ptr<Environment> closure_;
  bool is_initializer_;
};

}  // namespace LOX
#endif
