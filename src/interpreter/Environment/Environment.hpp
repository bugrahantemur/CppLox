#ifndef LOX_INTERPRETER_ENVIRONMENT
#define LOX_INTERPRETER_ENVIRONMENT

#include <cassert>
#include <optional>
#include <string>
#include <unordered_map>

#include "../../Types/Object/Object.hpp"
#include "../../Types/Token/Token.hpp"
#include "../../Utils/Arc.hpp"

namespace LOX::Interpreter {

namespace Details {

template <typename Key, typename Value>
class Environment {
 public:
  std::optional<Arc<Environment>> enclosing;

  Environment(std::optional<Arc<Environment>> const& enclosing)
      : enclosing(enclosing) {}

  Environment() : enclosing(std::nullopt) {}

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
      current = current->enclosing.value().get();
    }
    assert(current);
    return current;
  }

  std::unordered_map<Key, Value> map_;
};
}  // namespace Details

using Environment = Details::Environment<std::string, Types::Objects::Object>;

}  // namespace LOX::Interpreter

#endif
