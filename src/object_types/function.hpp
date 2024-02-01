#ifndef LOX_TYPES_FUNCTION
#define LOX_TYPES_FUNCTION

#include <memory>
#include <optional>

#include "../interpreter/environment.hpp"
#include "../syntax_types/statement.hpp"

namespace LOX::Objects {

struct LoxFunction {
  Statements::FunctionStmt declaration_;
  Arc<Environment> closure_;
  bool is_initializer_;
};

}  // namespace LOX::Objects

#endif
