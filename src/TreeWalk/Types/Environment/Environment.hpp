#ifndef LOX_TREEWALK_TYPES_ENVIRONMENT
#define LOX_TREEWALK_TYPES_ENVIRONMENT

#include <cassert>
#include <optional>
#include <string>
#include <unordered_map>

#include "../../../../submodules/RustyPtr/include/RustyPtr/Arc.hpp"
#include "../../../Common/Types/Objects/Object.hpp"

namespace LOX::TreeWalk::Types {

class Environment {
 public:
  std::optional<Arc<Environment>> enclosing;

  Environment(std::optional<Arc<Environment>> const& enclosing);

  Environment();

  auto define(std::string const& name, Common::Types::Object const& value)
      -> void;

  auto get_at(std::string const& name, std::size_t distance)
      -> Common::Types::Object;

  auto get(std::string const& name) -> Common::Types::Object;

  auto assign_at(std::string const& name, Common::Types::Object const& value,
                 std::size_t distance) -> void;

  auto assign(std::string const& name, Common::Types::Object const& value)
      -> void;

 private:
  auto ancestor(std::size_t distance) -> Environment*;

  std::unordered_map<std::string, Common::Types::Object> map;
};

}  // namespace LOX::TreeWalk::Types

#endif
