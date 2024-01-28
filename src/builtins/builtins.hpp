#ifndef LOX_BUILTINS
#define LOX_BUILTINS

#include <string>
#include <vector>

#include "../object_types/object.hpp"
#include "./clock.hpp"

namespace LOX::Builtins {

inline auto builtins() -> std::vector<std::pair<std::string, Object>> {
  return {{"clock", std::make_shared<Clock>()}, {"pi", 3.14159265359}};
}

}  // namespace LOX::Builtins

#endif
