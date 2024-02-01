#ifndef LOX_BUILTINS
#define LOX_BUILTINS

#include <string>
#include <vector>

#include "../Types/Object/Object.hpp"
#include "../Utils/Arcdyn.hpp"
#include "./Clock.hpp"

namespace LOX::Native {

inline auto builtins() -> std::vector<std::pair<std::string, Types::Object>> {
  return {
      {"clock", ArcDyn<Types::Objects::Builtins::FunctionInterface>{Clock{}}},
      {"pi", 3.14159265359}};
}

}  // namespace LOX::Native

#endif
