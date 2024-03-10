#include "./Truth.hpp"

#include <variant>

#include "../../../Types/Objects/Object.hpp"

namespace LOX::TreeWalk::Interpreter::Utils {

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

}  // namespace LOX::TreeWalk::Interpreter::Utils
