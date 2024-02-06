#include "./Instance.hpp"

#include "../../../Types/Environment/Environment.hpp"
#include "../../../Types/Objects/Object.hpp"
#include "../../Error/Error.hpp"

namespace LOX::Interpreter::Expressions::Instance {

using namespace Types::Objects;
using Types::Environment;

auto get(Arc<LoxInstance> const& instance, Token const& token) -> Object {
  if (auto const field{instance->fields_.find(token.lexeme)};
      field != instance->fields_.end()) {
    return field->second;
  }

  if (std::optional<LoxFunction> const method{
          instance->class_.find_method(token.lexeme)}) {
    Arc<Environment> env{Arc{Environment{method.value().closure_}}};

    env->define("this", instance);
    return Box{LoxFunction{method.value().declaration_, env,
                           method.value().is_initializer}};
  }

  throw Error{token.line, "Undefined property '" + token.lexeme + "'."};
}

auto set(Arc<LoxInstance>& instance, Token const& name, Object const& value)
    -> void {
  instance->fields_[name.lexeme] = value;
}

}  // namespace LOX::Interpreter::Expressions::Instance
