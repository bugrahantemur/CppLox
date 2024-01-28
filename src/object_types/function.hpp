#ifndef LOX_TYPES_FUNCTION
#define LOX_TYPES_FUNCTION

#include <memory>

#include "../interpreter/environment.hpp"
#include "../syntax_types/statement.hpp"

namespace LOX::Objects {

struct LoxFunction {
  Statements::FunctionStatement declaration_;
  std::shared_ptr<Environment> closure_;
  bool is_initializer_;
};

}  // namespace LOX::Objects
#endif
