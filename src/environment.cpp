#include "./environment.h"

#include <optional>
#include <string>

#include "./types/object.h"

auto Environment::define(std::string const& name, Object const& value) -> void {
  values_[name] = value;
}

auto Environment::get(std::string const& name) const -> std::optional<Object> {
  if (auto const found{values_.find(name)}; found != values_.end()) {
    return found->second;
  }
  return std::nullopt;
}
