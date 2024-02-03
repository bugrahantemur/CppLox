#include <exception>
#include <string>
#include <variant>

#include "../../../Types/Object/Object.hpp"

namespace LOX::Interpreter::Expressions::Equality {

struct NotComparableError : public std::exception {};

struct Equality {
  auto operator()(std::monostate, std::monostate) -> bool { return true; }
  auto operator()(bool left, bool right) -> bool { return left == right; }
  auto operator()(double left, double right) -> bool { return left == right; }
  auto operator()(std::string const& left, std::string const& right) -> bool {
    return left == right;
  }

  template <typename T, typename U>
  auto operator()(T const& left, U const& right) -> bool {
    throw NotComparableError{};
  }
};

auto is_equal(Object const& left, Object const& right) -> bool {
  return std::visit(Equality{}, left, right);
}

}  // namespace LOX::Interpreter::Expressions::Equality
