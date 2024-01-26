#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "../objects/object.hpp"
#include "../syntax/statement.hpp"
#include "../syntax/token.hpp"
#include "./environment.hpp"

namespace LOX::Interpreter {

auto interpret(std::vector<Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution,
               std::shared_ptr<Environment> const& env) -> void;

auto interpret(std::vector<Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution)
    -> void;

}  // namespace LOX::Interpreter

#endif
