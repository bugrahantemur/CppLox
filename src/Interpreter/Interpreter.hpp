#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include <unordered_map>
#include <vector>

#include "../../RustyPtr/include/RustyPtr/Arc.hpp"
#include "../Types/Objects/Object.hpp"
#include "../Types/Syntax/Statement.hpp"
#include "../Types/Tokens/Token.hpp"

namespace LOX::Types {
struct Environment;
}

namespace LOX::Interpreter {

auto interpret(
    std::vector<Types::Syntax::Statements::Statement> const& statements,
    std::unordered_map<Types::Tokens::Token, std::size_t> const& resolution,
    Arc<Types::Environment> const& env) -> void;

auto interpret(
    std::vector<Types::Syntax::Statements::Statement> const& statements,
    std::unordered_map<Types::Tokens::Token, std::size_t> const& resolution)
    -> void;

}  // namespace LOX::Interpreter

#endif
