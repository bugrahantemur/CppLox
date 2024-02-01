#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include <unordered_map>
#include <vector>

#include "../Types/Syntax/Statement.hpp"
#include "../Types/Token/Token.hpp"
#include "../Utils/Arc.hpp"
#include "./Environment.hpp"

namespace LOX::Interpreter {

auto interpret(std::vector<Types::Syntax::Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution,
               Arc<Environment> const& env) -> void;

auto interpret(std::vector<Types::Syntax::Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution)
    -> void;

}  // namespace LOX::Interpreter

#endif
