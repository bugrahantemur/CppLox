#ifndef LOX_TYPES_CLASS
#define LOX_TYPES_CLASS

#include <optional>
#include <string>
#include <unordered_map>

#include "../interpreter/error.hpp"
#include "../syntax/token.hpp"
#include "../utils/box.hpp"
#include "./function.hpp"
#include "./object.hpp"

namespace LOX::Objects {

struct LoxClass {
  std::string name_;
  std::optional<Box<LoxClass>> superclass_;
  std::unordered_map<std::string, LoxFunction> methods_;

  auto find_method(std::string const& name) const
      -> std::optional<LoxFunction> {
    if (auto const method{methods_.find(name)}; method != methods_.end()) {
      return method->second;
    }

    if (superclass_) {
      return superclass_.value()->find_method(name);
    }

    return std::nullopt;
  }
};

struct LoxInstance {
  LoxClass class_;
  std::unordered_map<std::string, Object> fields_;
};

inline auto get(std::shared_ptr<LoxInstance> const& instance,
                Token const& token) -> Object {
  if (auto const field{instance->fields_.find(token.lexeme_)};
      field != instance->fields_.end()) {
    return field->second;
  }

  if (auto const method{instance->class_.find_method(token.lexeme_)}) {
    auto const env{std::make_shared<Environment>(method.value().closure_)};
    env->define("this", instance);
    return LoxFunction{method.value().declaration_, env,
                       method.value().is_initializer_};
  }

  throw Interpreter::Error{token.line_,
                           "Undefined property '" + token.lexeme_ + "'."};
}

inline auto set(std::shared_ptr<LoxInstance> instance, Token const& name,
                Object const& value) -> void {
  instance->fields_[name.lexeme_] = value;
}
}  // namespace LOX::Objects
#endif
