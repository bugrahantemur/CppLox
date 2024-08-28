#include "./Execute.hpp"

#include <unordered_map>
#include <variant>

#include "../../../../submodules/RustyPtr/include/RustyPtr/Arc.hpp"
#include "../../../../submodules/RustyPtr/include/RustyPtr/Box.hpp"
#include "../../../Common/Types/Syntax/Statement.hpp"
#include "../../../Common/Types/Tokens/Token.hpp"
#include "../../../Common/Utils/Truth/Truth.hpp"
#include "../../Types/Environment/Environment.hpp"
#include "../../Types/Objects/Object.hpp"
#include "../Error/Error.hpp"
#include "../Expressions/Evaluate.hpp"
#include "../Utils/Return/Return.hpp"
#include "./Put/Put.hpp"

namespace LOX::TreeWalk::Interpreter::Statements {

using namespace LOX::Common::Types::Syntax::Statements;
using namespace LOX::Common::Types::Syntax::Expressions;
using namespace LOX::Common::Types::Tokens;
using LOX::Common::Types::Token;

using namespace LOX::TreeWalk::Types::Objects;
using LOX::TreeWalk::Types::Environment;

struct StatementExecutor {
  Arc<Environment> environment;
  std::unordered_map<Token, std::size_t> const& resolution;

  auto operator()(std::monostate) -> void {}

  auto operator()(Box<ExpressionStmt> const& stmt) -> void {
    static_cast<void>(
        Expressions::evaluate(stmt->expression, environment, resolution));
  }

  auto operator()(Box<PrintStmt> const& stmt) -> void {
    Object const value{
        Expressions::evaluate(stmt->expression, environment, resolution)};
    std::visit(Put{std::cout}, value);
  }

  auto operator()(Box<ReturnStmt> const& stmt) -> void {
    Object const value{
        Expressions::evaluate(stmt->value, environment, resolution)};

    throw Utils::Return{value};
  }

  auto operator()(Box<VariableStmt> const& stmt) -> void {
    Object const value{
        Expressions::evaluate(stmt->initializer, environment, resolution)};

    environment->define(stmt->name.lexeme, value);
  }

  auto operator()(Box<BlockStmt> const& stmt) -> void {
    // Create new environment with the current environment as its
    // enclosing environment
    auto env{Arc{Environment{environment}}};

    // Execute statements in the block with the new environment
    for (Statement const& statement : stmt->statements) {
      execute(statement, env, resolution);
    }
  }

  auto operator()(Box<FunctionStmt> const& stmt) -> void {
    environment->define(stmt->name.lexeme,
                        Box{LoxFunction{*stmt, environment, false}});
  }

  auto operator()(Box<ClassStmt> const& stmt) -> void {
    std::optional<Box<LoxClass>> superclass;
    if (stmt->super_class.name != Token::none()) {
      try {
        superclass = std::get<Box<LoxClass>>(Expressions::evaluate(
            Expression{stmt->super_class}, environment, resolution));
      } catch (std::bad_variant_access const&) {
        throw Error{stmt->super_class.name.line, "Superclass must be a class."};
      }
    }

    environment->define(stmt->name.lexeme, std::monostate{});

    if (superclass) {
      environment = Arc{Environment{environment}};
      environment->define("super", superclass.value());
    }

    std::unordered_map<std::string, LoxFunction> class_methods;
    for (Box<FunctionStmt> const& method : stmt->methods) {
      class_methods[method->name.lexeme] =
          LoxFunction{*method, environment, method->name.lexeme == "init"};
    }

    if (superclass) {
      environment = environment->enclosing.value();
    }

    environment->assign(
        stmt->name.lexeme,
        Box{LoxClass{stmt->name.lexeme, superclass, class_methods}});
  }

  auto operator()(Box<IfStmt> const& stmt) -> void {
    if (Common::Utils::is_truthy(
            Expressions::evaluate(stmt->condition, environment, resolution))) {
      std::visit(*this, stmt->then_branch);
    } else {
      std::visit(*this, stmt->else_branch);
    }
  }

  auto operator()(Box<WhileStmt> const& stmt) -> void {
    while (Common::Utils::is_truthy(
        Expressions::evaluate(stmt->condition, environment, resolution))) {
      std::visit(*this, stmt->body);
    }
  }
};

auto execute(Statement const& statement, Arc<Environment> environment,
             std::unordered_map<Token, std::size_t> const& resolution) -> void {
  std::visit(StatementExecutor{environment, resolution}, statement);
}

}  // namespace LOX::TreeWalk::Interpreter::Statements
