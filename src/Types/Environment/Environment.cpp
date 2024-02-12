#include "./Environment.hpp"

#include <cassert>
#include <optional>
#include <string>
#include <unordered_map>

#include "../../../RustyPtr/include/RustyPtr/Arc.hpp"
#include "../Objects/Object.hpp"

namespace LOX::Types {

Environment::Environment(std::optional<Arc<Environment>> const& enclosing)
    : enclosing(enclosing) {}

Environment::Environment() : enclosing(std::nullopt) {}

auto Environment::define(std::string const& name, Objects::Object const& value)
    -> void {
  map[name] = value;
}

auto Environment::get_at(std::string const& name, std::size_t distance)
    -> Objects::Object {
  return ancestor(distance)->map.at(name);
}

auto Environment::get(std::string const& name) -> Objects::Object {
  return get_at(name, 0);
}

auto Environment::assign_at(std::string const& name,
                            Objects::Object const& value, std::size_t distance)
    -> void {
  ancestor(distance)->map[name] = value;
}

auto Environment::assign(std::string const& name, Objects::Object const& value)
    -> void {
  assign_at(name, value, 0);
}

auto Environment::ancestor(std::size_t distance) -> Environment* {
  auto current{this};
  for (std::size_t i{0}; i < distance; ++i) {
    assert(current);
    current = current->enclosing.value().get();
  }
  assert(current);
  return current;
}

}  // namespace LOX::Types
