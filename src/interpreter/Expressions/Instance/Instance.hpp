#ifndef LOX_INTERPRETER_EXPRESSIONS_INSTANCE
#define LOX_INTERPRETER_EXPRESSIONS_INSTANCE

#include "../../../Types/Objects/Class.hpp"
#include "../../../Types/Objects/Function.hpp"
#include "../../../Types/Objects/Object.hpp"

namespace LOX::Interpreter::Expressions::Instance {

using namespace Types::Objects;

auto get(Arc<LoxInstance> const& instance, Token const& token) -> Object;

auto set(Arc<LoxInstance> instance, Token const& name, Object const& value)
    -> void;

}  // namespace LOX::Interpreter::Expressions::Instance
#endif
