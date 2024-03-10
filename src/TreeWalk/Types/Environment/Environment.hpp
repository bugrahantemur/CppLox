#ifndef LOX_TREEWALK_TYPES_ENVIRONMENT
#define LOX_TREEWALK_TYPES_ENVIRONMENT

#include <cassert>
#include <optional>
#include <string>
#include <unordered_map>

#include "../../../../submodules/RustyPtr/include/RustyPtr/Arc.hpp"
#include "../Objects/Object.hpp"

namespace LOX::TreeWalk::Types {

class Environment {
 public:
  std::optional<Arc<Environment>> enclosing;

  Environment(std::optional<Arc<Environment>> const& enclosing);

  Environment();

  auto define(std::string const& name, Objects::Object const& value) -> void;

  auto get_at(std::string const& name, std::size_t distance) -> Objects::Object;

  auto get(std::string const& name) -> Objects::Object;

  auto assign_at(std::string const& name, Objects::Object const& value,
                 std::size_t distance) -> void;

  auto assign(std::string const& name, Objects::Object const& value) -> void;

 private:
  auto ancestor(std::size_t distance) -> Environment*;

  std::unordered_map<std::string, Objects::Object> map;
};

}  // namespace LOX::TreeWalk::Types

#endif
