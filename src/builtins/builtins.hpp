#ifndef LOX_BUILTINS
#define LOX_BUILTINS

#include <string>
#include <vector>

#include "../Utils/arcdyn.hpp"
#include "../object_types/object.hpp"
#include "./clock.hpp"

namespace LOX::Builtins {

inline auto builtins() -> std::vector<std::pair<std::string, Object>> {
  return {{"clock", ArcDyn<FunctionInterface>{Clock{}}}, {"pi", 3.14159265359}};
}

}  // namespace LOX::Builtins

#endif
