#include "./Resolver.hpp"

#include <string>

#include "../Builtins/names.hpp"
#include "./Error/Error.hpp"

namespace LOX::Resolver {

using Types::Syntax::Expression;
using Types::Syntax::Statement;
using namespace Types::Syntax::Statements;
using namespace Types::Syntax::Expressions;

enum class FunctionType { NONE, FUNCTION, INITIALIZER, METHOD };
enum class ClassType { NONE, CLASS, SUBCLASS };

struct Resolve {
  std::unordered_map<Token, std::size_t>& resolution_;
  std::vector<std::unordered_map<std::string, bool>>& scopes_;

  FunctionType& current_function_type_;
  ClassType& current_class_type_;

  auto operator()(std::monostate) -> void {}

  auto operator()(Box<ExpressionStmt> const& stmt) -> void {
    resolve(stmt->expression_);
  }

  auto operator()(Box<PrintStmt> const& stmt) -> void {
    resolve(stmt->expression_);
  }

  auto operator()(Box<ReturnStmt> const& stmt) -> void {
    if (current_function_type_ == FunctionType::NONE) {
      throw Error{stmt->keyword_.line_, "Can't return from top-level code."};
    }

    if (current_function_type_ == FunctionType::INITIALIZER) {
      throw Error{stmt->keyword_.line_,
                  "Can't return a value from an initializer."};
    }

    resolve(stmt->value_);
  }

  auto operator()(Box<VariableStmt> const& stmt) -> void {
    declare(stmt->name_);
    resolve(stmt->initializer_);
    define(stmt->name_);
  }

  auto operator()(Box<BlockStmt> const& stmt) -> void {
    begin_scope();
    resolve(stmt->statements_);
    end_scope();
  }

  auto operator()(Box<FunctionStmt> const& stmt) -> void {
    declare(stmt->name_);
    define(stmt->name_);

    resolve(*stmt, FunctionType::FUNCTION);
  }

  auto operator()(Box<ClassStmt> const& stmt) -> void {
    ClassType const enclosing_class_type{current_class_type_};
    current_class_type_ = ClassType::CLASS;

    declare(stmt->name_);
    define(stmt->name_);

    if (stmt->super_class_.name_ != Token::none()) {
      if (stmt->super_class_.name_ == stmt->name_) {
        throw Error{stmt->super_class_.name_.line_,
                    "A class can't inherit from itself."};
      }

      current_class_type_ = ClassType::SUBCLASS;
      resolve(stmt->super_class_);

      begin_scope();
      scopes_.back()["super"] = true;
    }

    begin_scope();
    scopes_.back()["this"] = true;

    for (Box<FunctionStmt> const& method : stmt->methods_) {
      resolve(*method, method->name_.lexeme_ == "init"
                           ? FunctionType::INITIALIZER
                           : FunctionType::METHOD);
    }

    end_scope();
    if (stmt->super_class_.name_ != Token::none()) {
      end_scope();
    }

    current_class_type_ = enclosing_class_type;
  }

  auto operator()(Box<IfStmt> const& stmt) -> void {
    resolve(stmt->condition_);
    resolve(stmt->then_branch_);
    resolve(stmt->else_branch_);
  }

  auto operator()(Box<WhileStmt> const& stmt) -> void {
    resolve(stmt->condition_);
    resolve(stmt->body_);
  }

  auto operator()(Box<LiteralExpr> const& expr) -> void {}

  auto operator()(Box<SuperExpr> const& expr) -> void {
    if (current_class_type_ == ClassType::NONE) {
      throw Resolver::Error{expr->keyword_.line_,
                            "Can't use 'super' outside of a class."};
    }

    if (current_class_type_ != ClassType::SUBCLASS) {
      throw Resolver::Error{expr->keyword_.line_,
                            "Can't use 'super' in a class with no superclass."};
    }

    resolve(expr->keyword_);
  }

  auto operator()(Box<ThisExpr> const& expr) -> void {
    if (current_class_type_ == ClassType::NONE) {
      throw Resolver::Error{expr->keyword_.line_,
                            "Can't use 'this' outside of a class."};
    }

    resolve(expr->keyword_);
  }

  auto operator()(Box<VariableExpr> const& expr) -> void {
    if (!scopes_.empty()) {
      if (auto const found{scopes_.back().find(expr->name_.lexeme_)};
          found != scopes_.back().end() && found->second == false) {
        throw Resolver::Error{
            expr->name_.line_,
            "Can't read local variable in its own initializer."};
      }
    }
    resolve(expr->name_);
  }

  auto operator()(Box<AssignmentExpr> const& expr) -> void {
    resolve(expr->value_);
    resolve(expr->name_);
  }

  auto operator()(Box<BinaryExpr> const& expr) -> void {
    resolve(expr->left_);
    resolve(expr->right_);
  }

  auto operator()(Box<CallExpr> const& expr) -> void {
    resolve(expr->callee_);

    for (Expression const& argument : expr->arguments_) {
      resolve(argument);
    }
  }

  auto operator()(Box<GetExpr> const& expr) -> void { resolve(expr->object_); }

  auto operator()(Box<GroupingExpr> const& expr) -> void {
    resolve(expr->expression_);
  }

  auto operator()(Box<LogicalExpr> const& expr) -> void {
    resolve(expr->left_);
    resolve(expr->right_);
  }

  auto operator()(Box<SetExpr> const& expr) -> void {
    resolve(expr->value_);
    resolve(expr->object_);
  }

  auto operator()(Box<UnaryExpr> const& expr) -> void { resolve(expr->right_); }

 private:
  auto resolve(Statement const& stmt) -> void { std::visit(*this, stmt); }

  auto resolve(Expression const& expr) -> void { std::visit(*this, expr); }

  auto resolve(std::vector<Statement> const& stmts) -> void {
    for (auto const& stmt : stmts) {
      resolve(stmt);
    }
  }

  auto resolve(Token const& name) -> void {
    for (auto scope{scopes_.crbegin()}; scope != scopes_.crend(); ++scope) {
      if (scope->find(name.lexeme_) != scope->end()) {
        resolution_[name] = std::distance(scopes_.crbegin(), scope);
        return;
      }
    }
  }

  auto resolve(FunctionStmt const& stmt, FunctionType function_type) -> void {
    FunctionType const enclosing_function{current_function_type_};
    current_function_type_ = function_type;

    begin_scope();

    for (Token const& param : stmt.params_) {
      declare(param);
      define(param);
    }
    resolve(stmt.body_);

    end_scope();

    current_function_type_ = enclosing_function;
  }

  auto declare(Token const& name) const -> void {
    if (!scopes_.empty()) {
      auto& scope{scopes_.back()};
      if (scope.find(name.lexeme_) != scope.end()) {
        throw Resolver::Error{name.line_,
                              "Already a variable with this name declared in "
                              "this scope."};
      }
      scopes_.back()[name.lexeme_] = false;
    }
  }

  auto define(Token const& name) const -> void {
    if (!scopes_.empty()) {
      scopes_.back()[name.lexeme_] = true;
    }
  }

  auto begin_scope() -> void { scopes_.emplace_back(); }

  auto end_scope() -> void { scopes_.pop_back(); }
};

auto preamble() -> std::unordered_map<std::string, bool> {
  std::unordered_map<std::string, bool> scope;
  for (auto const& name : Native::names()) {
    scope[name] = true;
  }
  return scope;
}

auto resolve(std::vector<Statement> const& statements)
    -> std::unordered_map<Token, std::size_t> {
  auto function_type{FunctionType::NONE};
  auto class_type{ClassType::NONE};
  std::vector<std::unordered_map<std::string, bool>> scopes{preamble()};
  std::unordered_map<Token, std::size_t> resolution;

  Resolve resolver{resolution, scopes, function_type, class_type};

  for (Statement const& stmt : statements) {
    std::visit(resolver, stmt);
  }

  return resolution;
}

}  // namespace LOX::Resolver
