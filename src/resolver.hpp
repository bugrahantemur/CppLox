#include <unordered_map>
#include <vector>

#include "./interpreter.hpp"
#include "./types/statement.hpp"
#include "./utils/error.hpp"

namespace Resolve {
struct Error : LoxError {
  std::size_t line_;
  std::string message_;

  auto report() const -> void final {
    std::cerr << "[line " << line_ << "] Resolver error: " << message_ << '\n';
  }
};

auto error(std::size_t line, std::string const& message) -> Error {
  return Error{line, message};
}
}  // namespace Resolve

class Resolver {
 public:
  Resolver(Interpreter& interpreter) : interpreter_{interpreter}, scopes_{} {}

  auto operator()(std::monostate) -> void {}

  auto operator()(ExpressionStatement const& stmt) -> void {
    resolve(stmt.expression_);
  }

  auto operator()(PrintStatement const& stmt) -> void {
    resolve(stmt.expression_);
  }

  auto operator()(ReturnStatement const& stmt) -> void {
    if (!std::holds_alternative<std::monostate>(stmt.value_)) {
      resolve(stmt.value_);
    }
  }

  auto operator()(VariableStatement const& stmt) -> void {
    declare(stmt.name_);
    resolve(stmt.initializer_);
    define(stmt.name_);
  }

  auto operator()(Box<BlockStatement> const& stmt) -> void {
    begin_scope();
    resolve(stmt);
    end_scope();
  }

  auto operator()(Box<FunctionStatement> const& stmt) -> void {
    declare(stmt->name_);
    define(stmt->name_);

    resolve_function(*stmt);
  }

  auto operator()(Box<IfStatement> const& stmt) -> void {
    resolve(stmt->condition_);
    resolve(stmt->then_branch_);
    if (!std::holds_alternative<std::monostate>(stmt->else_branch_)) {
      resolve(stmt->else_branch_);
    }
  }

  auto operator()(Box<WhileStatement> const& stmt) -> void {
    resolve(stmt->condition_);
    resolve(stmt->body_);
  }

  auto operator()(LiteralExpression const& expr) -> void {}

  auto operator()(VariableExpression const& expr) -> void {
    if (!scopes_.empty() && !scopes_.back()[expr.name_.lexeme_]) {
      throw Resolve::error(expr.name_.line_,
                           "Can't read local variable in its own initializer.");
    }

    resolve_local(expr.name_);
  }

  auto operator()(Box<AssignmentExpression> const& expr) -> void {
    resolve(expr->value_);
    resolve_local(expr->name_);
  }

  auto operator()(Box<BinaryExpression> const& expr) -> void {
    resolve(expr->left_);
    resolve(expr->right_);
  }

  auto operator()(Box<CallExpression> const& expr) -> void {
    resolve(expr->callee_);

    for (Expression const& argument : expr->arguments_) {
      resolve(argument);
    }
  }

  auto operator()(Box<GroupingExpression> const& expr) -> void {
    resolve(expr->expression_);
  }

  auto operator()(Box<LogicalExpression> const& expr) -> void {
    resolve(expr->left_);
    resolve(expr->right_);
  }

  auto operator()(Box<UnaryExpression> const& expr) -> void {
    resolve(expr->right_);
  }

 private:
  auto begin_scope() -> void { scopes_.emplace_back(); }

  auto end_scope() -> void { scopes_.pop_back(); }

  auto declare(std::string const& name) -> void {
    if (!scopes_.empty()) {
      scopes_.back()[name] = false;
    }
  }

  auto define(std::string const& name) -> void {
    if (!scopes_.empty()) {
      scopes_.back()[name] = true;
    }
  }

  auto resolve(Expression const& expr) -> void { std::visit(*this, expr); }

  auto resolve(Statement const& stmt) -> void { std::visit(*this, stmt); }

  auto resolve(std::vector<Statement> const& statements) -> void {
    for (auto const& statement : statements) {
      resolve(statement);
    }
  }

  auto resolve_local(Token const& name) -> void {
    for (auto scope = scopes_.crbegin(); scope != scopes_.crend(); ++scope) {
      if (scope->find(name.lexeme_) != scope->end()) {
        interpreter_.resolve(name, std::distance(scopes_.crbegin(), scope));
        return;
      }
    }
  }

  auto resolve_function(FunctionStatement const& stmt) -> void {
    begin_scope();

    for (std::string const& param : stmt.params_) {
      declare(param);
      define(param);
    }
    resolve(stmt.body_);
    end_scope();
  }

  Interpreter& interpreter_;
  std::vector<std::unordered_map<std::string, bool>> scopes_;
};
