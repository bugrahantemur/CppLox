#ifndef LOX_TREEWALK_INTERPRETER_STATEMENT_EXECUTE
#define LOX_TREEWALK_INTERPRETER_STATEMENT_EXECUTE

#include <unordered_map>

#include "../../../../submodules/RustyPtr/include/RustyPtr/Arc.hpp"
#include "../../../Common/Types/Syntax/Statement.hpp"
#include "../../../Common/Types/Tokens/Token.hpp"

namespace LOX::TreeWalk::Types {
class Environment;
}

namespace LOX::TreeWalk::Interpreter::Statements {

auto execute(Common::Types::Syntax::Statements::Statement const& statement,
             Arc<Types::Environment> environment,
             std::unordered_map<LOX::Common::Types::Token, std::size_t> const&
                 resolution) -> void;

}  // namespace LOX::TreeWalk::Interpreter::Statements

#endif
