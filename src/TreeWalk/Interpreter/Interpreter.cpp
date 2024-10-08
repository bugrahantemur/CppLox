#include "./Interpreter.hpp"

#include <vector>

#include "../../../submodules/RustyPtr/include/RustyPtr/Arc.hpp"
#include "../../Common/Types/Objects/Object.hpp"
#include "../../Common/Types/Syntax/Statement.hpp"
#include "../../Common/Types/Tokens/Token.hpp"
#include "../Builtins/Builtins.hpp"
#include "../Types/Environment/Environment.hpp"
#include "./Statements/Execute.hpp"

namespace LOX::TreeWalk::Interpreter {

using namespace LOX::Common::Types::Syntax::Statements;
using namespace LOX::Common::Types::Tokens;
using LOX::Common::Types::Token;

using LOX::TreeWalk::Types::Environment;

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
