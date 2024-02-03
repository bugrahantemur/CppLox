#ifndef LOX_INTERPRETER_STATEMENT_EXECUTOR
#define LOX_INTERPRETER_STATEMENT_EXECUTOR

#include <unordered_map>
#include <variant>

#include "../../Types/Object/Class.hpp"
#include "../../Types/Object/Function.hpp"
#include "../../Types/Object/Object.hpp"
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
using Types::Syntax::Statement;

struct StatementExecutor {
  Arc<Environment> environment_;
  std::unordered_map<Token, std::size_t> const& resolution_;

  auto operator()(std::monostate) -> void {}

  auto operator()(Box<ExpressionStmt> const& stmt) -> void {
    static_cast<void>(
        std::visit(Expressions::ExpressionEvaluator{environment_, resolution_},
                   stmt->expression_));
  }

  auto operator()(Box<PrintStmt> const& stmt) -> void {
    Object const value{std::visit(
        ExpressionEvaluator{environment_, resolution_}, stmt->expression_)};
    std::visit(Put{std::cout}, value);
  }

  auto operator()(Box<ReturnStmt> const& stmt) -> void {
    Object const value{std::visit(
        ExpressionEvaluator{environment_, resolution_}, stmt->value_)};

    throw Utils::Return{value};
  }

  auto operator()(Box<VariableStmt> const& stmt) -> void {
    Object const value{std::visit(
        ExpressionEvaluator{environment_, resolution_}, stmt->initializer_)};

    environment_->define(stmt->name_.lexeme_, value);
  }

  auto operator()(Box<BlockStmt> const& stmt) -> void {
    // Create new environment with the current environment as its
    // enclosing environment
    auto env{Arc{Environment{environment_}}};

    // Execute statements in the block with the new environment
    for (Statement const& stmt : stmt->statements_) {
      std::visit(StatementExecutor{env, resolution_}, stmt);
    }
  }

  auto operator()(Box<FunctionStmt> const& stmt) -> void {
    environment_->define(stmt->name_.lexeme_,
                         Box{LoxFunction{*stmt, environment_, false}});
  }

  auto operator()(Box<ClassStmt> const& stmt) -> void {
    std::optional<Box<LoxClass>> superclass;
    if (stmt->super_class_.name_ != Token::none()) {
      try {
        superclass = std::get<Box<LoxClass>>(
            ExpressionEvaluator{environment_, resolution_}(stmt->super_class_));
      } catch (std::bad_variant_access const&) {
        throw Error{stmt->super_class_.name_.line_,
                    "Superclass must be a class."};
      }
    }

    environment_->define(stmt->name_.lexeme_, std::monostate{});

    if (superclass) {
      environment_ = Arc{Environment{environment_}};
      environment_->define("super", superclass.value());
    }

    std::unordered_map<std::string, LoxFunction> class_methods;
    for (Box<FunctionStmt> const& method : stmt->methods_) {
      class_methods[method->name_.lexeme_] =
          LoxFunction{*method, environment_, method->name_.lexeme_ == "init"};
    }

    if (superclass) {
      environment_ = environment_->enclosing_.value();
    }

    environment_->assign(
        stmt->name_.lexeme_,
        Box{LoxClass{stmt->name_.lexeme_, superclass, class_methods}});
  }

  auto operator()(Box<IfStmt> const& stmt) -> void {
    if (Utils::is_truthy(
            std::visit(ExpressionEvaluator{environment_, resolution_},
                       stmt->condition_))) {
      std::visit(*this, stmt->then_branch_);
    } else {
      std::visit(*this, stmt->else_branch_);
    }
  }

  auto operator()(Box<WhileStmt> const& stmt) -> void {
    while (Utils::is_truthy(std::visit(
        ExpressionEvaluator{environment_, resolution_}, stmt->condition_))) {
      std::visit(*this, stmt->body_);
    }
  }
};
}  // namespace LOX::Interpreter::Statements
#endif
