#ifndef LOX_TREEWALK_INTERPRETER_EXPRESSIONS_CLASS
#define LOX_TREEWALK_INTERPRETER_EXPRESSIONS_CLASS

#include <optional>
#include <string>

#include "../../../Types/Objects/Object.hpp"

namespace LOX::TreeWalk::Interpreter::Expressions::Class {

auto get(Arc<Types::Objects::LoxInstance> const& instance,
         LOX::Common::Types::Token const& token) -> Types::Objects::Object;

auto set(Arc<Types::Objects::LoxInstance>& instance,
         LOX::Common::Types::Token const& name,
         Types::Objects::Object const& value) -> void;

auto find_method(Types::Objects::LoxClass const& klass, std::string const& name)
    -> std::optional<Types::Objects::LoxFunction>;

}  // namespace LOX::TreeWalk::Interpreter::Expressions::Class
#endif
