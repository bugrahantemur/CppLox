#ifndef LOX_INTERPRETER_UTILS_RETURN
#define LOX_INTERPRETER_UTILS_RETURN

#include "../../../Types/Objects/Object.hpp"

namespace LOX::TreeWalk::Interpreter::Utils {

struct Return {
  Types::Objects::Object value;
};

}  // namespace LOX::TreeWalk::Interpreter::Utils

#endif
