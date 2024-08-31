#ifndef LOX_TREEWALK_INTERPRETER
#define LOX_TREEWALK_INTERPRETER

#include <unordered_map>
#include <vector>

#include "../../../submodules/RustyPtr/include/RustyPtr/Arc.hpp"
#include "../../Common/Types/Syntax/Statement.hpp"
#include "../../Common/Types/Tokens/Token.hpp"

namespace LOX::TreeWalk::Types {
class Environment;
}

namespace LOX::TreeWalk::Interpreter {

auto interpret(
    std::vector<Common::Types::Syntax::Statements::Statement> const& statements,
    std::unordered_map<LOX::Common::Types::Token, std::size_t> const&
        resolution,
    Arc<Types::Environment> const& env) -> void;

auto interpret(
    std::vector<Common::Types::Syntax::Statements::Statement> const& statements,
    std::unordered_map<LOX::Common::Types::Token, std::size_t> const&
        resolution) -> void;

}  // namespace LOX::TreeWalk::Interpreter

#endif
