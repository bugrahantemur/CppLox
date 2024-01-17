#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../types/environment.hpp"
#include "../types/object.hpp"
#include "../types/statement.hpp"
#include "../types/token.hpp"

namespace Interpreter {

auto interpret(std::vector<Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution,
               std::shared_ptr<Environment> const& env) -> void;

auto interpret(std::vector<Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution)
    -> void;

}  // namespace Interpreter

#endif
