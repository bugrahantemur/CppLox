#ifndef LOX_TREEWALK_BUILTINS_CLOCK
#define LOX_TREEWALK_BUILTINS_CLOCK

#include <string>
#include <unordered_map>
#include <vector>

#include "../../../../Common/Types/Objects/Object.hpp"

namespace LOX::TreeWalk::Types {
class Environment;
}

namespace LOX::TreeWalk::Native::Functions {

struct Clock : public Common::Types::Objects::Builtins::FunctionInterface {
  [[nodiscard]] auto arity() const -> std::size_t final;

  [[nodiscard]] auto to_string() const -> std::string final;

  auto operator()(Arc<Types::Environment> const& environment,
                  std::unordered_map<LOX::Common::Types::Token,
                                     std::size_t> const& resolution,
                  std::vector<Common::Types::Object> const& args) const
      -> Common::Types::Object final;
};

}  // namespace LOX::TreeWalk::Native::Functions

#endif
