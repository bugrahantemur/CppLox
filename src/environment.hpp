#ifndef LOX_ENVIRONMENT
#define LOX_ENVIRONMENT

#include <map>
#include <memory>

#include "./types/object.hpp"
#include "./types/token.hpp"

namespace {
template <typename Key, typename Value>
class env {
 public:
  env(std::shared_ptr<env> const& enclosing)
      : enclosing_(enclosing), global_env_(enclosing_->global_env_) {}

  env()
      : enclosing_(nullptr),
        global_env_(nullptr)  // TODO
  {}

  auto define(Key const& name, Value const& value) -> void {
    map_[name] = value;
  }

  auto get(Key const& name) -> Value {
    if (auto const found = local_resolution_.find(name);
        found != local_resolution_.end()) {
      return ancestor(found->second).at(name);
    }

    return global_env_->map_.at(name);
  }

  auto assign(Key const& name, Value const& value) -> void {
    if (auto const found{local_resolution_.find(name)};
        found != local_resolution_.end()) {
      ancestor(found->second).at(name) = value;
      return;
    }

    global_env_->map_.at(name) = value;
  }

  auto resolve(Key const& name, std::size_t distance) -> void {
    local_resolution_[name] = distance;
  }

 private:
  auto ancestor(std::size_t distance) -> std::map<Key, Value>& {
    auto current{this};
    for (std::size_t i = 0; i < distance; ++i) {
      current = current->enclosing_.get();
    }
    return current->map_;
  }

  std::map<Key, std::size_t> local_resolution_;

  std::shared_ptr<env> enclosing_;

  std::shared_ptr<env> global_env_;

  std::map<Key, Value> map_;
};
}  // namespace

using Environment = env<Token, Object>;

#endif
