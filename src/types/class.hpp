#ifndef LOX_TYPES_CLASS
#define LOX_TYPES_CLASS

#include <optional>
#include <string>
#include <unordered_map>

#include "../utils/box.hpp"
#include "../utils/error.hpp"
#include "./function.hpp"
#include "./object.hpp"
#include "./token.hpp"

struct LoxClass {
  std::string name_;
  std::optional<Box<LoxClass>> superclass_;
  std::unordered_map<std::string, LoxFunction> methods_;
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

  if (auto const method{instance->class_.methods_.find(token.lexeme_)};
      method != instance->class_.methods_.end()) {
    auto const env{std::make_shared<Environment>(method->second.closure_)};
    env->define("this", instance);
    return Box{LoxFunction{method->second.declaration_, env,
                           method->second.is_initializer_}};
  }

  throw RuntimeError{token.line_,
                     "Undefined property '" + token.lexeme_ + "'."};
}

inline auto set(std::shared_ptr<LoxInstance> instance, Token const& name,
                Object const& value) -> void {
  instance->fields_[name.lexeme_] = value;
}

#endif
