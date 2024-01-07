#ifndef LOX_TYPES_CLASS
#define LOX_TYPES_CLASS

#include <string>
#include <unordered_map>

#include "../utils/error.hpp"
#include "./function.hpp"
#include "./object.hpp"
#include "./token.hpp"

struct LoxClass {
  std::string name_;
  std::unordered_map<std::string, LoxFunction> methods_;
};

class LoxInstance {
 public:
  LoxInstance(LoxClass const& klass) : class_{klass} {}

  auto to_string() const -> std::string {
    return "<instance of " + class_.name_ + ">";
  }

  auto get(Token const& token) const -> Object {
    if (auto const field{fields_.find(token.lexeme_)}; field != fields_.end()) {
      return field->second;
    }

    if (auto const method{class_.methods_.find(token.lexeme_)};
        method != class_.methods_.end()) {
      return method->second;
    }

    throw RuntimeError{token.line_,
                       "Undefined property '" + token.lexeme_ + "'."};
  }

  auto set(Token const& name, Object const& value) -> void {
    fields_[name.lexeme_] = value;
  }

 private:
  LoxClass class_;
  std::unordered_map<std::string, Object> fields_;
};

#endif
