#ifndef LOX_ENVIRONMENT
#define LOX_ENVIRONMENT

#include <cassert>
#include <map>
#include <memory>

#include "./types/object.hpp"
#include "./types/token.hpp"
#include "./utils/error.hpp"

namespace {
template <typename Key, typename Value>
class env {
 public:
  std::shared_ptr<env> enclosing_;

  explicit env(std::shared_ptr<env> const& enclosing) : enclosing_(enclosing) {}

  env() : enclosing_(nullptr) {}

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
  auto ancestor(std::size_t distance) -> env* {
    auto current{this};
    for (std::size_t i = 0; i < distance; ++i) {
      assert(current);
      current = current->enclosing_.get();
    }
    assert(current);
    return current;
  }

  std::unordered_map<Key, Value> map_;
};
}  // namespace

using Environment = env<std::string, Object>;

#endif
