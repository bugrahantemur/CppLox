#ifndef LOX_INTERPRETER_EXPRESSIONS_CLASS
#define LOX_INTERPRETER_EXPRESSIONS_CLASS

#include <optional>
#include <string>

#include "../../../Types/Objects/Object.hpp"

namespace LOX::Interpreter::Expressions::Class {

auto get(Arc<Types::Objects::LoxInstance> const& instance,
         Types::Tokens::Token const& token) -> Types::Objects::Object;

auto set(Arc<Types::Objects::LoxInstance>& instance,
         Types::Tokens::Token const& name, Types::Objects::Object const& value)
    -> void;

auto find_method(Types::Objects::LoxClass const& klass, std::string const& name)
    -> std::optional<Types::Objects::LoxFunction>;

}  // namespace LOX::Interpreter::Expressions::Class
#endif
