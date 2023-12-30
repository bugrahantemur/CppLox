#include <unordered_map>
#include <vector>

#include "./interpreter.hpp"
#include "./types/statement.hpp"

class Resolver {
 public:
  auto operator()(std::monostate) -> void {}

  auto operator()(Box<BlockStatement> const& stmt) -> void {
    begin_scope();
    resolve(stmt);
    end_scope();
  }

  auto operator()(VariableStatement const& stmt) -> void {
    declare(stmt.name_);
    resolve(stmt.initializer_);
    define(stmt.name_);
  }

  auto operator()(VariableExpression const& expr) -> void {
    if (!scopes_.empty() && !scopes_.back()[expr.name_.lexeme_]) {
      error(expr.name_, "Can't read local variable in its own initializer.");
    }

    resolve_local(expr, expr.name_.lexeme_);
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

  template <typename T>
  auto resolve(T const& t) -> void {
    std::visit(*this, t);
  }

  auto resolve(std::vector<Statement> const& statements) -> void {
    for (auto const& statement : statements) {
      resolve(statement);
    }
  }

  auto resolve_local(Expression const& expr, std::string const& name) -> void {
    for (auto scope = scopes_.crbegin(); scope != scopes_.crend(); ++scope) {
      if (scope->find(name) != scope->end()) {
        interpreter_.resolve(expr, std::distance(scopes_.crbegin(), scope));
        return;
      }
    }
  }

  Interpreter const& interpreter_;
  std::vector<std::unordered_map<std::string, bool>> scopes_;
};
