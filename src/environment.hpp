#ifndef LOX_ENVIRONMENT
#define LOX_ENVIRONMENT

#include <map>

#include "./types/object.hpp"

namespace {
template <typename Key, typename Value>
class env {
 public:
  env(env* enclosing) : enclosing_(enclosing) {}

  env() : env(nullptr) {}

  auto define(Key const& name, Value const& value) -> void {
    map_[name] = value;
  }

  auto get(Key const& name) const -> Value {
    if (auto const found = map_.find(name); found != map_.end()) {
      return found->second;
    }

    if (enclosing_) {
      return enclosing_->get(name);
    }

    throw std::out_of_range{out_of_range_message};
  }

  auto assign(Key const& name, Value const& value) -> void {
    if (auto const found = map_.find(name); found != map_.end()) {
      found->second = value;
      return;
    }

    if (enclosing_) {
      enclosing_->assign(name, value);
      return;
    }

    throw std::out_of_range{out_of_range_message};
  }

 private:
  char const* out_of_range_message{"Environment map"};

  env* enclosing_;

  std::map<Key, Value> map_;
};
}  // namespace

using Environment = env<std::string const, Object>;

#endif
