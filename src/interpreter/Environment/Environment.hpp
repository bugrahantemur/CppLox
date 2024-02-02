#ifndef LOX_Environment
#define LOX_Environment

#include <cassert>
#include <map>
#include <optional>

#include "../../Utils/Arc.hpp"

namespace LOX::Interpreter::Details {

template <typename Key, typename Value>
class Environment {
 public:
  std::optional<Arc<Environment>> enclosing_;

  Environment(std::optional<Arc<Environment>> const& enclosing)
      : enclosing_(enclosing) {}

  Environment() : enclosing_(std::nullopt) {}

  auto define(Key const& name, Value const& value) -> void {
    map_[name] = value;
  }

  auto get_at(Key const& name, std::size_t distance) -> Value {
    return ancestor(distance)->map_.at(name);
  }

  auto get(Key const& name) -> Value { return get_at(name, 0); }

  auto assign_at(Key const& name, Value const& value, std::size_t distance)
      -> void {
    ancestor(distance)->map_[name] = value;
  }

  auto assign(Key const& name, Value const& value) -> void {
    assign_at(name, value, 0);
  }

 private:
  auto ancestor(std::size_t distance) -> Environment* {
    auto current{this};
    for (std::size_t i{0}; i < distance; ++i) {
      assert(current);
      current = current->enclosing_.value().get();
    }
    assert(current);
    return current;
  }

  std::unordered_map<Key, Value> map_;
};

}  // namespace LOX::Interpreter::Details
#endif
