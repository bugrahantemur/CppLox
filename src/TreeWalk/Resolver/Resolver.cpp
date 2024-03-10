#include "./Resolver.hpp"

#include <string>

#include "../Builtins/Builtins.hpp"
#include "./Error/Error.hpp"

namespace LOX::TreeWalk::Resolver {

using LOX::Common::Types::Token;

using namespace LOX::TreeWalk::Types::Syntax::Statements;
using namespace LOX::TreeWalk::Types::Syntax::Expressions;
using namespace LOX::Common::Types::Tokens;

enum class FunctionType { NONE, FUNCTION, INITIALIZER, METHOD };
enum class ClassType { NONE, CLASS, SUBCLASS };

struct Resolve {
  std::unordered_map<Token, std::size_t>& resolution;
  std::vector<std::unordered_map<std::string, bool>>& scopes;

  FunctionType& current_function_type;
  ClassType& current_class_type;

  auto operator()(std::monostate) -> void {}

  auto operator()(Box<ExpressionStmt> const& stmt) -> void {
    resolve(stmt->expression);
  }

  auto operator()(Box<PrintStmt> const& stmt) -> void {
    resolve(stmt->expression);
  }

  auto operator()(Box<ReturnStmt> const& stmt) -> void {
    if (current_function_type == FunctionType::NONE) {
      throw Error{stmt->keyword.line, "Can't return from top-level code."};
    }

    if (current_function_type == FunctionType::INITIALIZER) {
      throw Error{stmt->keyword.line,
                  "Can't return a value from an initializer."};
    }

    resolve(stmt->value);
  }

  auto operator()(Box<VariableStmt> const& stmt) -> void {
    declare(stmt->name);
    resolve(stmt->initializer);
    define(stmt->name);
  }

  auto operator()(Box<BlockStmt> const& stmt) -> void {
    begin_scope();
    resolve(stmt->statements);
    end_scope();
  }

  auto operator()(Box<FunctionStmt> const& stmt) -> void {
    declare(stmt->name);
    define(stmt->name);

    resolve(*stmt, FunctionType::FUNCTION);
  }

  auto operator()(Box<ClassStmt> const& stmt) -> void {
    ClassType const enclosing_class_type{current_class_type};
    current_class_type = ClassType::CLASS;

    declare(stmt->name);
    define(stmt->name);

    if (stmt->super_class.name != Token::none()) {
      if (stmt->super_class.name == stmt->name) {
        throw Error{stmt->super_class.name.line,
                    "A class can't inherit from itself."};
      }

      current_class_type = ClassType::SUBCLASS;
      resolve(stmt->super_class);

      begin_scope();
      scopes.back()["super"] = true;
    }

    begin_scope();
    scopes.back()["this"] = true;

    for (Box<FunctionStmt> const& method : stmt->methods) {
      resolve(*method, method->name.lexeme == "init" ? FunctionType::INITIALIZER
                                                     : FunctionType::METHOD);
    }

    end_scope();
    if (stmt->super_class.name != Token::none()) {
      end_scope();
    }

    current_class_type = enclosing_class_type;
  }

  auto operator()(Box<IfStmt> const& stmt) -> void {
    resolve(stmt->condition);
    resolve(stmt->then_branch);
    resolve(stmt->else_branch);
  }

  auto operator()(Box<WhileStmt> const& stmt) -> void {
    resolve(stmt->condition);
    resolve(stmt->body);
  }

  auto operator()(Box<LiteralExpr> const& expr) -> void {}

  auto operator()(Box<SuperExpr> const& expr) -> void {
    if (current_class_type == ClassType::NONE) {
      throw Resolver::Error{expr->keyword.line,
                            "Can't use 'super' outside of a class."};
    }

    if (current_class_type != ClassType::SUBCLASS) {
      throw Resolver::Error{expr->keyword.line,
                            "Can't use 'super' in a class with no superclass."};
    }

    resolve(expr->keyword);
  }

  auto operator()(Box<ThisExpr> const& expr) -> void {
    if (current_class_type == ClassType::NONE) {
      throw Resolver::Error{expr->keyword.line,
                            "Can't use 'this' outside of a class."};
    }

    resolve(expr->keyword);
  }

  auto operator()(Box<VariableExpr> const& expr) -> void {
    if (!scopes.empty()) {
      if (auto const found{scopes.back().find(expr->name.lexeme)};
          found != scopes.back().end() && found->second == false) {
        throw Resolver::Error{
            expr->name.line,
            "Can't read local variable in its own initializer."};
      }
    }
    resolve(expr->name);
  }

  auto operator()(Box<AssignmentExpr> const& expr) -> void {
    resolve(expr->value);
    resolve(expr->name);
  }

  auto operator()(Box<BinaryExpr> const& expr) -> void {
    resolve(expr->left);
    resolve(expr->right);
  }

  auto operator()(Box<CallExpr> const& expr) -> void {
    resolve(expr->callee);

    for (Expression const& argument : expr->arguments) {
      resolve(argument);
    }
  }

  auto operator()(Box<GetExpr> const& expr) -> void { resolve(expr->object); }

  auto operator()(Box<GroupingExpr> const& expr) -> void {
    resolve(expr->expression);
  }

  auto operator()(Box<LogicalExpr> const& expr) -> void {
    resolve(expr->left);
    resolve(expr->right);
  }

  auto operator()(Box<SetExpr> const& expr) -> void {
    resolve(expr->value);
    resolve(expr->object);
  }

  auto operator()(Box<UnaryExpr> const& expr) -> void { resolve(expr->right); }

 private:
  auto resolve(Statement const& stmt) -> void { std::visit(*this, stmt); }

  auto resolve(Expression const& expr) -> void { std::visit(*this, expr); }

  auto resolve(std::vector<Statement> const& stmts) -> void {
    for (auto const& stmt : stmts) {
      resolve(stmt);
    }
  }

  auto resolve(Token const& name) -> void {
    for (auto scope{scopes.crbegin()}; scope != scopes.crend(); ++scope) {
      if (scope->find(name.lexeme) != scope->end()) {
        resolution[name] = std::distance(scopes.crbegin(), scope);
        return;
      }
    }
  }

  auto resolve(FunctionStmt const& stmt, FunctionType function_type) -> void {
    FunctionType const enclosing_function{current_function_type};
    current_function_type = function_type;

    begin_scope();

    for (Token const& param : stmt.params) {
      declare(param);
      define(param);
    }
    resolve(stmt.body);

    end_scope();

    current_function_type = enclosing_function;
  }

  auto declare(Token const& name) const -> void {
    if (!scopes.empty()) {
      auto& scope{scopes.back()};
      if (scope.find(name.lexeme) != scope.end()) {
        throw Resolver::Error{name.line,
                              "Already a variable with this name declared in "
                              "this scope."};
      }
      scopes.back()[name.lexeme] = false;
    }
  }

  auto define(Token const& name) const -> void {
    if (!scopes.empty()) {
      scopes.back()[name.lexeme] = true;
    }
  }

  auto begin_scope() -> void { scopes.emplace_back(); }

  auto end_scope() -> void { scopes.pop_back(); }
};

auto make_preamble_scope() -> std::unordered_map<std::string, bool> {
  std::unordered_map<std::string, bool> scope;
  for (auto const& builtin : Native::builtins()) {
    scope[builtin.first] = true;
  }
  return scope;
}

auto resolve(std::vector<Statement> const& statements)
    -> std::unordered_map<Token, std::size_t> {
  auto function_type{FunctionType::NONE};
  auto class_type{ClassType::NONE};
  std::vector<std::unordered_map<std::string, bool>> scopes{
      make_preamble_scope()};
  std::unordered_map<Token, std::size_t> resolution;

  Resolve resolver{resolution, scopes, function_type, class_type};

  for (Statement const& stmt : statements) {
    std::visit(resolver, stmt);
  }

  return resolution;
}

}  // namespace LOX::TreeWalk::Resolver
