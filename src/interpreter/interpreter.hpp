#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include <unordered_map>
#include <vector>

#include "../Types/Object/Object.hpp"
#include "../Types/Syntax/Statement.hpp"
#include "../Types/Token/Token.hpp"
#include "../Utils/Arc.hpp"
#include "./Environment/Environment.hpp"

namespace LOX::Interpreter {

using Environment = Details::Environment<std::string, Types::Object>;

auto interpret(std::vector<Types::Syntax::Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution,
               Arc<Environment> const& env) -> void;

auto interpret(std::vector<Types::Syntax::Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution)
    -> void;

}  // namespace LOX::Interpreter

#endif
