
#ifndef LOX_TYPES_CLASS
#define LOX_TYPES_CLASS

#include <optional>
#include <string>
#include <unordered_map>

#include "../../Types/Token/Token.hpp"
#include "../../Utils/Box.hpp"
#include "./Function.hpp"
#include "./Object.hpp"

namespace LOX::Types::Objects {

struct LoxClass {
  std::string name;
  std::optional<Box<LoxClass>> super_class;
  std::unordered_map<std::string, LoxFunction> methods;

  auto find_method(std::string const& name) const
      -> std::optional<LoxFunction> {
    if (auto const method{methods.find(name)}; method != methods.end()) {
      return method->second;
    }

    if (super_class) {
      return super_class.value()->find_method(name);
    }

    return std::nullopt;
  }
};

struct LoxInstance {
  LoxClass class_;
  std::unordered_map<std::string, Object> fields_;
};

}  // namespace LOX::Types::Objects
#endif
