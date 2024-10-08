#include "./Class.hpp"

#include "../../../../Common/Types/Objects/Object.hpp"
#include "../../../Types/Environment/Environment.hpp"
#include "../../Error/Error.hpp"

namespace LOX::TreeWalk::Interpreter::Expressions::Class {

using namespace LOX::Common::Types::Tokens;
using LOX::Common::Types::Token;

using namespace LOX::Common::Types::Objects;
using LOX::Common::Types::Object;
using LOX::TreeWalk::Types::Environment;

auto get(Arc<LoxInstance> const& instance, Token const& token) -> Object {
  if (auto const field{instance->fields.find(token.lexeme)};
      field != instance->fields.end()) {
    return field->second;
  }

  if (std::optional<LoxFunction> const method{
          find_method(instance->class_, token.lexeme)}) {
    Arc<Environment> env{Arc{Environment{method.value().closure}}};

    env->define("this", instance);
    return Box{LoxFunction{method.value().declaration, env,
                           method.value().is_initializer}};
  }

  throw Error{token.line, "Undefined property '" + token.lexeme + "'."};
}

auto set(Arc<LoxInstance>& instance, Token const& name, Object const& value)
    -> void {
  instance->fields[name.lexeme] = value;
}

auto find_method(LoxClass const& klass, std::string const& name)
    -> std::optional<LoxFunction> {
  if (auto const method{klass.methods.find(name)};
      method != klass.methods.end()) {
    return method->second;
  }

  if (klass.super_class) {
    return find_method(*klass.super_class.value(), name);
  }

  return std::nullopt;
}
}  // namespace LOX::TreeWalk::Interpreter::Expressions::Class
