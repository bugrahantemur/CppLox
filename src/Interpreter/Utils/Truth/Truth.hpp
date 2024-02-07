#ifndef LOX_INTERPRETER_UTILS_TRUTH
#define LOX_INTERPRETER_UTILS_TRUTH

#include <variant>

#include "../../../Types/Objects/Object.hpp"

namespace LOX::Interpreter::Utils {

auto is_truthy(Types::Objects::Object const& obj) -> bool;

}  // namespace LOX::Interpreter::Utils

#endif
