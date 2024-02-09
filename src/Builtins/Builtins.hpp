#ifndef LOX_BUILTINS
#define LOX_BUILTINS

#include <string>
#include <vector>

#include "../Types/Objects/Object.hpp"
#include "../Utils/Arcdyn.hpp"
#include "./Constants/Math.hpp"
#include "./Functions/Clock/Clock.hpp"

namespace LOX::Native {

inline auto builtins()
    -> std::vector<std::pair<std::string, Types::Objects::Object>> {
  return {
      {"clock",
       ArcDyn<Types::Objects::Builtins::FunctionInterface>{Functions::Clock{}}},
      {"pi", Constants::Math::PI}};
}

}  // namespace LOX::Native

#endif
