#ifndef LOX_TYPES_FUNCTION
#define LOX_TYPES_FUNCTION

#include <memory>
#include <optional>

#include "../../Interpreter/Environment.hpp"
#include "../Syntax/Statement.hpp"

namespace LOX::Types::Objects {

struct LoxFunction {
  Syntax::Statements::FunctionStmt declaration_;
  Arc<Environment> closure_;
  bool is_initializer_;
};

}  // namespace LOX::Types::Objects

#endif
