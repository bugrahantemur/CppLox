#ifndef LOX_ENVIRONMENT
#define LOX_ENVIRONMENT

#include <map>
#include <string>

template <typename T>
class Environment {
 public:
  auto define(std::string const& name, T const& value) -> void {
    values_[name] = value;
  }

  auto get(std::string const& name) const -> T { return values_.at(name); }

  auto assign(std::string const& name, T const& value) -> void {
    values_.at(name) = value;
  }

 private:
  std::map<std::string, T> values_;
};

#endif
