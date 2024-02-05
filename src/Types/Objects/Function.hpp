#ifndef LOX_TYPES_FUNCTION
#define LOX_TYPES_FUNCTION

#include <memory>
#include <optional>

#include "../../Interpreter/Interpreter.hpp"
#include "../Syntax/Statement.hpp"

namespace LOX::Types::Objects {

struct LoxFunction {
  Syntax::Statements::FunctionStmt declaration_;
  Arc<Interpreter::Environment> closure_;
  bool is_initializer;
};

}  // namespace LOX::Types::Objects

#endif
