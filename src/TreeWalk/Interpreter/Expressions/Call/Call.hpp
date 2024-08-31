#ifndef LOX_TREEWALK_TREEWALK_INTERPRETER_EXPRESSIONS_CALL
#define LOX_TREEWALK_TREEWALK_INTERPRETER_EXPRESSIONS_CALL

#include <exception>
#include <unordered_map>
#include <vector>

#include "../../../../../submodules/RustyPtr/include/RustyPtr/Arc.hpp"
#include "../../../../Common/Types/Objects/Object.hpp"
#include "../../../../Common/Types/Tokens/Token.hpp"

namespace LOX::TreeWalk::Types {
class Environment;
}

namespace LOX::TreeWalk::Interpreter::Expressions {

struct UncallableError : public std::exception {};

auto call(Arc<Types::Environment> environment,
          std::unordered_map<LOX::Common::Types::Token, std::size_t> const&
              resolution,
          std::vector<Common::Types::Object> const& args,
          Common::Types::Object const& callee) -> Common::Types::Object;

auto arity_of(Common::Types::Object const& callee) -> std::size_t;

}  // namespace LOX::TreeWalk::Interpreter::Expressions

#endif
