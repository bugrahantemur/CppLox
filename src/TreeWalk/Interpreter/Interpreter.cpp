#include "./Interpreter.hpp"

#include <chrono>
#include <concepts>
#include <iostream>
#include <string>
#include <variant>

#include "../../../submodules/RustyPtr/include/RustyPtr/Arc.hpp"
#include "../../../submodules/RustyPtr/include/RustyPtr/Box.hpp"
#include "../../Common/Types/Tokens/Token.hpp"
#include "../Builtins/Builtins.hpp"
#include "../Types/Environment/Environment.hpp"
#include "../Types/Objects/Object.hpp"
#include "../Types/Syntax/Expression.hpp"
#include "../Types/Syntax/Statement.hpp"
#include "./Error/Error.hpp"
#include "./Statements/Execute.hpp"

namespace LOX::TreeWalk::Interpreter {

using Types::Environment;
using namespace LOX::TreeWalk::Types::Objects;
using namespace LOX::Types::Tokens;
using LOX::Types::Token;
using namespace LOX::TreeWalk::Types::Syntax::Expressions;
using namespace LOX::TreeWalk::Types::Syntax::Statements;

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
  for (auto const& statement : statements) {
    Statements::execute(statement, environment, resolution);
  }
}

auto interpret(std::vector<Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution)
    -> void {
  Arc<Environment> const preamble_environment{make_preamble_environment()};

  interpret(statements, resolution, preamble_environment);
}

}  // namespace LOX::TreeWalk::Interpreter
