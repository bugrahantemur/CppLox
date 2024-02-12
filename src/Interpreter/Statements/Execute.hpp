#ifndef LOX_INTERPRETER_STATEMENT_EXECUTE
#define LOX_INTERPRETER_STATEMENT_EXECUTE

#include <unordered_map>

#include "../../../RustyPtr/include/RustyPtr/Arc.hpp"
#include "../../Types/Syntax/Statement.hpp"
#include "../../Types/Tokens/Token.hpp"

namespace LOX::Types {
struct Environment;
}

namespace LOX::Interpreter::Statements {

auto execute(
    Types::Syntax::Statements::Statement const& statement,
    Arc<Types::Environment> environment,
    std::unordered_map<Types::Tokens::Token, std::size_t> const& resolution)
    -> void;

}  // namespace LOX::Interpreter::Statements

#endif
