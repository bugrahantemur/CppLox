#ifndef LOX_INTERPRETER_EXPRESSIONS_INSTANCE
#define LOX_INTERPRETER_EXPRESSIONS_INSTANCE

#include "../../../Types/Object/Class.hpp"
#include "../../../Types/Object/Function.hpp"
#include "../../../Types/Object/Object.hpp"

namespace LOX::Interpreter::Expressions::Instance {

using namespace Types::Objects;

auto get(Arc<LoxInstance> const& instance, Token const& token) -> Object {
  if (auto const field{instance->fields_.find(token.lexeme_)};
      field != instance->fields_.end()) {
    return field->second;
  }

  if (std::optional<LoxFunction> const method{
          instance->class_.find_method(token.lexeme_)}) {
    Arc<Environment> env{Arc{Environment{method.value().closure_}}};

    env->define("this", instance);
    return Box{LoxFunction{method.value().declaration_, env,
                           method.value().is_initializer_}};
  }

  throw Error{token.line_, "Undefined property '" + token.lexeme_ + "'."};
}

auto set(Arc<LoxInstance> instance, Token const& name, Object const& value)
    -> void {
  instance->fields_[name.lexeme_] = value;
}

}  // namespace LOX::Interpreter::Expressions::Instance
#endif
