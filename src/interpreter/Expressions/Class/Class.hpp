#ifndef LOX_INTERPRETER_EXPRESSIONS_CLASS
#define LOX_INTERPRETER_EXPRESSIONS_CLASS

#include <optional>
#include <string>

#include "../../../Types/Objects/Object.hpp"

namespace LOX::Interpreter::Expressions::Class {

using namespace Types::Objects;

auto get(Arc<LoxInstance> const& instance, Token const& token) -> Object;

auto set(Arc<LoxInstance>& instance, Token const& name, Object const& value)
    -> void;

auto find_method(LoxClass const& klass, std::string const& name)
    -> std::optional<LoxFunction>;

}  // namespace LOX::Interpreter::Expressions::Class
#endif
