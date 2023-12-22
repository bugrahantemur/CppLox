#ifndef LOX_ENVIRONMENT
#define LOX_ENVIRONMENT

#include <map>

template <typename Key, typename Value>
class Environment {
 public:
  Environment() : enclosing_(nullptr) {}

  Environment(Environment* enclosing) : enclosing_(enclosing) {}

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
  char const* out_of_range_message{
      "Not found in current or enclosing environments."};

  Environment* enclosing_;

  std::map<Key, Value> map_;
};

#endif
