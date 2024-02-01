#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include <unordered_map>
#include <vector>

#include "../syntax_types/statement.hpp"
#include "../token/token.hpp"
#include "../utils/arc.hpp"
#include "./environment.hpp"

namespace LOX::Interpreter {

auto interpret(std::vector<Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution,
               Arc<Environment> const& env) -> void;

auto interpret(std::vector<Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution)
    -> void;

}  // namespace LOX::Interpreter

#endif
