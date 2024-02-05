#ifndef LOX_INTERPRETER_STATEMENT_EXECUTOR
#define LOX_INTERPRETER_STATEMENT_EXECUTOR

#include <unordered_map>
#include <variant>

#include "../../Types/Objects/Class.hpp"
#include "../../Types/Objects/Function.hpp"
#include "../../Types/Objects/Object.hpp"
#include "../../Types/Syntax/Statement.hpp"
#include "../../Types/Token/Token.hpp"
#include "../../Utils/Arc.hpp"
#include "../../Utils/Box.hpp"
#include "../Environment/Environment.hpp"
#include "../Error/Error.hpp"
#include "../Expressions/ExpressionEvaluator.hpp"
#include "../Utils/Truth/Truth.hpp"
#include "./Put/Put.hpp"

namespace LOX::Interpreter::Statements {

using namespace Types::Syntax::Statements;
using Expressions::ExpressionEvaluator;

struct StatementExecutor {
  Arc<Environment> environment;
  std::unordered_map<Token, std::size_t> const& resolution;

  auto operator()(std::monostate) -> void {}

  auto operator()(Box<ExpressionStmt> const& stmt) -> void {
    static_cast<void>(
        std::visit(Expressions::ExpressionEvaluator{environment, resolution},
                   stmt->expression));
  }

  auto operator()(Box<PrintStmt> const& stmt) -> void {
    Object const value{std::visit(ExpressionEvaluator{environment, resolution},
                                  stmt->expression)};
    std::visit(Put{std::cout}, value);
  }

  auto operator()(Box<ReturnStmt> const& stmt) -> void {
    Object const value{
        std::visit(ExpressionEvaluator{environment, resolution}, stmt->value)};

    throw Utils::Return{value};
  }

  auto operator()(Box<VariableStmt> const& stmt) -> void {
    Object const value{std::visit(ExpressionEvaluator{environment, resolution},
                                  stmt->initializer)};

    environment->define(stmt->name.lexeme, value);
  }

  auto operator()(Box<BlockStmt> const& stmt) -> void {
    // Create new environment with the current environment as its
    // enclosing environment
    auto env{Arc{Environment{environment}}};

    // Execute statements in the block with the new environment
    for (Statement const& stmt : stmt->statements) {
      std::visit(StatementExecutor{env, resolution}, stmt);
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
        superclass = std::get<Box<LoxClass>>(
            ExpressionEvaluator{environment, resolution}(stmt->super_class));
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
    if (Utils::is_truthy(std::visit(
            ExpressionEvaluator{environment, resolution}, stmt->condition))) {
      std::visit(*this, stmt->then_branch);
    } else {
      std::visit(*this, stmt->else_branch);
    }
  }

  auto operator()(Box<WhileStmt> const& stmt) -> void {
    while (Utils::is_truthy(std::visit(
        ExpressionEvaluator{environment, resolution}, stmt->condition))) {
      std::visit(*this, stmt->body);
    }
  }
};
}  // namespace LOX::Interpreter::Statements
#endif
