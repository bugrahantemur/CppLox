#ifndef LOX_ENVIRONMENT
#define LOX_ENVIRONMENT

#include <map>
#include <string>

#include "./types/object.h"

class Environment {
 public:
  auto define(std::string const& name, Object const& value) -> void;

  auto get(std::string const& name) const -> std::optional<Object>;

 private:
  std::map<std::string, Object> values_;
};

#endif
