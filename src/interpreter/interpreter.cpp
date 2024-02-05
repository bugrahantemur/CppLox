#include "./Interpreter.hpp"

#include <chrono>
#include <concepts>
#include <iostream>
#include <string>
#include <variant>

#include "../Builtins/Builtins.hpp"
#include "../Types/Objects/Class.hpp"
#include "../Types/Objects/Function.hpp"
#include "../Types/Objects/Object.hpp"
#include "../Types/Syntax/Expression.hpp"
#include "../Types/Syntax/Statement.hpp"
#include "../Types/Token/Token.hpp"
#include "../Utils/Arc.hpp"
#include "../Utils/Box.hpp"
#include "./Error/Error.hpp"
#include "./Statements/StatementExecutor.hpp"

namespace LOX::Interpreter {

using namespace Types::Syntax::Statements;
using namespace Types::Syntax::Expressions;

using namespace Types::Objects;

auto make_preamble_environment() -> Arc<Environment> {
  auto env{Arc{Environment{}}};

  for (auto const& name : Native::builtins()) {
    env->define(name.first, name.second);
  }

  return env;
}

auto interpret(std::vector<Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution,
               Arc<Environment> const& environment) -> void {
  for (auto const& stmt : statements) {
    std::visit(Statements::StatementExecutor{environment, resolution}, stmt);
  }
}

auto interpret(std::vector<Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution)
    -> void {
  Arc<Environment> const preamble_environment{make_preamble_environment()};

  interpret(statements, resolution, preamble_environment);
}

}  // namespace LOX::Interpreter
