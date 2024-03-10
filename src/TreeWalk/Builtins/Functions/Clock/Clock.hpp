#ifndef LOX_TREEWALK_BUILTINS_CLOCK
#define LOX_TREEWALK_BUILTINS_CLOCK

#include <string>
#include <unordered_map>
#include <vector>

#include "../../../Types/Objects/Object.hpp"

namespace LOX::TreeWalk::Types {
struct Environment;
}

namespace LOX::TreeWalk::Native::Functions {

struct Clock : public Types::Objects::Builtins::FunctionInterface {
  auto arity() const -> std::size_t final;
  auto to_string() const -> std::string final;
  auto operator()(Arc<Types::Environment> const& environment,
                  std::unordered_map<LOX::Common::Types::Token,
                                     std::size_t> const& resolution,
                  std::vector<Types::Objects::Object> const& args) const
      -> Types::Objects::Object final;
};

}  // namespace LOX::TreeWalk::Native::Functions

#endif
