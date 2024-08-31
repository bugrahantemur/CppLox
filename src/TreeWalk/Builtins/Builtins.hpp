#ifndef LOX_TREEWALK_BUILTINS
#define LOX_TREEWALK_BUILTINS

#include <string>
#include <vector>

#include "../../../submodules/RustyPtr/include/RustyPtr/ArcDyn.hpp"
#include "../../Common/Types/Objects/Object.hpp"
#include "./Constants/Math.hpp"
#include "./Functions/Clock/Clock.hpp"

namespace LOX::TreeWalk::Native {

inline auto builtins()
    -> std::vector<std::pair<std::string, Common::Types::Object>> {
  return {{"clock",
           ArcDyn<Common::Types::Objects::Builtins::FunctionInterface>{
               Functions::Clock{}}},
          {"pi", Constants::Math::PI}};
}

}  // namespace LOX::TreeWalk::Native

#endif
