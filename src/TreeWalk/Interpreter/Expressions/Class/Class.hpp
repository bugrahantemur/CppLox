#ifndef LOX_TREEWALK_INTERPRETER_EXPRESSIONS_CLASS
#define LOX_TREEWALK_INTERPRETER_EXPRESSIONS_CLASS

#include <optional>
#include <string>

#include "../../../../Common/Types/Objects/Object.hpp"

namespace LOX::TreeWalk::Interpreter::Expressions::Class {

auto get(Arc<Common::Types::Objects::LoxInstance> const& instance,
         LOX::Common::Types::Token const& token) -> Common::Types::Object;

auto set(Arc<Common::Types::Objects::LoxInstance>& instance,
         LOX::Common::Types::Token const& name,
         Common::Types::Object const& value) -> void;

auto find_method(Common::Types::Objects::LoxClass const& klass,
                 std::string const& name)
    -> std::optional<Common::Types::Objects::LoxFunction>;

}  // namespace LOX::TreeWalk::Interpreter::Expressions::Class

#endif
