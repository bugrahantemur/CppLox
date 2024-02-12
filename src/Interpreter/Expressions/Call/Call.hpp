#ifndef LOX_INTERPRETER_EXPRESSIONS_CALL
#define LOX_INTERPRETER_EXPRESSIONS_CALL

#include <exception>
#include <unordered_map>
#include <vector>

#include "../../../../RustyPtr/include/RustyPtr/Arc.hpp"
#include "../../../Types/Objects/Object.hpp"
#include "../../../Types/Tokens/Token.hpp"

namespace LOX::Types {
struct Environment;
}

namespace LOX::Interpreter::Expressions {

struct UncallableError : public std::exception {};

auto call(
    Arc<Types::Environment> environment,
    std::unordered_map<Types::Tokens::Token, std::size_t> const& resolution,
    std::vector<Types::Objects::Object> const& args,
    Types::Objects::Object const& callee) -> Types::Objects::Object;

auto arity_of(Types::Objects::Object const& callee) -> std::size_t;

}  // namespace LOX::Interpreter::Expressions

#endif
