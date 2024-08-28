#ifndef LOX_COMMON_UTILS_TRUTH
#define LOX_COMMON_UTILS_TRUTH

#include <variant>

namespace LOX::Common::Utils {

struct Truth {
  auto operator()(std::monostate none) -> bool {
    (void)none;
    return false;
  }

  auto operator()(bool b) -> bool { return b; }

  template <typename T>
  auto operator()(T const& number) -> bool {
    (void)number;

    return true;
  }
};

template <typename T>
auto is_truthy(T const& t) -> bool {
  return std::visit(Truth{}, t);
}

}  // namespace LOX::Common::Utils

#endif
