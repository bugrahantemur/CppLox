#ifndef LOX_INTERPRETER_UTILS_TRUTH
#define LOX_INTERPRETER_UTILS_TRUTH

#include <variant>

#include "../../../Types/Object/Object.hpp"

namespace LOX::Interpreter::Utils {

struct Truth {
  auto operator()(std::monostate) -> bool { return false; }

  auto operator()(bool b) -> bool { return b; }

  template <typename T>
  auto operator()(T const& number) -> bool {
    return true;
  }
};

auto is_truthy(Types::Objects::Object const& obj) -> bool {
  return std::visit(Truth{}, obj);
}

}  // namespace LOX::Interpreter::Utils

#endif
