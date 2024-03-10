#include "./Equality.hpp"

#include <concepts>
#include <exception>
#include <variant>

#include "../../../Types/Objects/Object.hpp"

namespace LOX::TreeWalk::Interpreter::Expressions {

struct Equality {
  template <std::equality_comparable T>
  auto operator()(T const& left, T const& right) -> bool {
    return left == right;
  }

  template <typename T, typename U>
  auto operator()(T const& left, U const& right) -> bool {
    throw NotComparableError{};
  }
};

auto is_equal(Types::Objects::Object const& left,
              Types::Objects::Object const& right) -> bool {
  return std::visit(Equality{}, left, right);
}

}  // namespace LOX::TreeWalk::Interpreter::Expressions
