#include "./interpreter.hpp"

#include <chrono>
#include <concepts>
#include <iostream>
#include <string>
#include <variant>

#include "../Builtins/builtins.hpp"
#include "../Token/Token.hpp"
#include "../Utils/arc.hpp"
#include "../Utils/box.hpp"
#include "../object_types/class.hpp"
#include "../object_types/function.hpp"
#include "../object_types/object.hpp"
#include "../syntax_types/expression.hpp"
#include "../syntax_types/statement.hpp"
#include "./Error/Error.hpp"

namespace LOX::Interpreter {

using namespace LOX::Statements;
using namespace LOX::Expressions;
using namespace LOX::Objects;

struct Return {
  Object value_;
};

template <typename... Objs>
auto check_number_operand(Token const& token, Objs... operands) -> void {
  std::array const ops{operands...};
  if (std::all_of(std::begin(ops), std::end(ops), [](Object const& obj) {
        return !std::holds_alternative<double>(obj);
      })) {
    throw Error{token.line_, sizeof...(operands) > 1
                                 ? "Operands must be numbers."
                                 : "Operand must be a number."};
  }
}

auto get(Arc<LoxInstance> const& instance, Token const& token) -> Object {
  if (auto const field{instance->fields_.find(token.lexeme_)};
      field != instance->fields_.end()) {
    return field->second;
  }

  if (std::optional<LoxFunction> const method{
          instance->class_.find_method(token.lexeme_)}) {
    Arc<Environment> env{Arc{Environment{method.value().closure_}}};

    env->define("this", instance);
    return Box{LoxFunction{method.value().declaration_, env,
                           method.value().is_initializer_}};
  }

  throw Error{token.line_, "Undefined property '" + token.lexeme_ + "'."};
}

auto set(Arc<LoxInstance> instance, Token const& name, Object const& value)
    -> void {
  instance->fields_[name.lexeme_] = value;
}

struct NotComparableError : public std::exception {};
struct Equality {
  auto operator()(std::monostate, std::monostate) -> bool { return true; }
  auto operator()(bool left, bool right) -> bool { return left == right; }
  auto operator()(double left, double right) -> bool { return left == right; }
  auto operator()(std::string const& left, std::string const& right) -> bool {
    return left == right;
  }

  template <typename T, typename U>
  auto operator()(T const& left, U const& right) -> bool {
    throw NotComparableError{};
  }
};

auto is_equal(Object const& left, Object const& right) -> bool {
  return std::visit(Equality{}, left, right);
}

template <typename OStream>
struct Put {
  Put(OStream& out) : out_{out} {}

  auto operator()(std::monostate) -> OStream& { return put("nil"); }

  auto operator()(std::string const& str) -> OStream& { return put(str); }

  auto operator()(double const number) -> OStream& { return put(number); }

  auto operator()(bool const b) -> OStream& {
    return b ? put("true") : put("false");
  }

  auto operator()(Box<LoxFunction> const& func) -> OStream& {
    return put("<fn " + func->declaration_.name_.lexeme_ + ">");
  }

  auto operator()(Box<LoxClass> const& klass) -> OStream& {
    return put("<class " + klass->name_ + ">");
  }

  auto operator()(Arc<LoxInstance> const& instance) -> OStream& {
    return put("<instance of " + instance->class_.name_ + ">");
  }

  auto operator()(ArcDyn<Builtins::FunctionInterface> const& func) -> OStream& {
    return put("<builtin-fn " + func->to_string() + ">");
  }

 private:
  template <typename T>
  auto put(T const& obj) -> OStream& {
    out_ << obj << '\n';
    return out_;
  }

  OStream& out_;
};

struct Truth {
  auto operator()(std::monostate) -> bool { return false; }

  auto operator()(bool b) -> bool { return b; }

  template <typename T>
  auto operator()(T const& number) -> bool {
    return true;
  }
};

auto is_truthy(Object const& obj) -> bool { return std::visit(Truth{}, obj); }

struct UncallableError : public std::exception {};
struct Arity {
  auto operator()(Box<LoxFunction> const& func) -> std::size_t {
    return func->declaration_.params_.size();
  }

  auto operator()(Box<LoxClass> const& klass) -> std::size_t {
    if (auto const initializer{klass->methods_.find("init")};
        initializer != klass->methods_.end()) {
      return (*this)(initializer->second);
    }
    return 0;
  }

  auto operator()(ArcDyn<Builtins::FunctionInterface> const& func)
      -> std::size_t {
    return func->arity();
  }

  template <typename T>
  auto operator()(T const& t) -> std::size_t {
    throw UncallableError{};
  }
};

struct Call {
  Arc<Environment> environment_;
  std::unordered_map<Token, std::size_t> const& resolution_;
  std::vector<Object> const& args_;

  auto operator()(Box<LoxFunction>& func) -> Object {
    auto env{Arc{Environment{func->closure_}}};

    std::size_t const arity{Arity{}(func)};

    for (std::size_t i = 0; i < arity; ++i) {
      env->define(func->declaration_.params_.at(i).lexeme_, args_.at(i));
    }

    try {
      Interpreter::interpret(func->declaration_.body_, resolution_, env);
    } catch (Return const& ret) {
      if (func->is_initializer_) {
        return env->get_at("this", 0);
      }

      return ret.value_;
    }

    return func->is_initializer_ ? env->get_at("this", 0) : std::monostate{};
  }

  auto operator()(Box<LoxClass> const& klass) -> Object {
    auto const instance{Arc{LoxInstance{*klass, {}}}};

    if (auto const initializer{klass->methods_.find("init")};
        initializer != klass->methods_.end()) {
      auto env{Arc{Environment{initializer->second.closure_}}};
      env->define("this", instance);
      auto const init{
          Box{LoxFunction{initializer->second.declaration_, env, true}}};
      (*this)(init);
    }

    return instance;
  }

  auto operator()(ArcDyn<Builtins::FunctionInterface> const& func) -> Object {
    return (*func)(args_);
  }

  template <typename T>
  auto operator()(T const& t) -> Object {
    throw UncallableError{};
  }
};

struct ExpressionEvaluator {
  Arc<Environment> environment_;
  std::unordered_map<Token, std::size_t> const& resolution_;

  [[nodiscard]] auto operator()(std::monostate) -> Object {
    return std::monostate{};
  }

  [[nodiscard]] auto operator()(Box<LiteralExpr> const& expr) -> Object {
    return std::visit([](auto const& value) -> Object { return value; },
                      expr->value_);
  }

  [[nodiscard]] auto operator()(Box<SuperExpr> const& expr) -> Object {
    std::size_t const distance{resolution_.at(expr->keyword_)};

    auto const superclass{
        std::get<Box<LoxClass>>(environment_->get_at("super", distance))};

    auto const instance{
        std::get<Arc<LoxInstance>>(environment_->get_at("this", distance - 1))};

    std::optional<LoxFunction> const method{
        superclass->find_method(expr->method_.lexeme_)};

    if (!method) {
      throw Error{expr->method_.line_,
                  "Undefined property '" + expr->method_.lexeme_ + "'"};
    }

    auto env{Arc{Environment{method.value().closure_}}};
    env->define("this", instance);
    return Box{LoxFunction{method.value().declaration_, env,
                           method.value().is_initializer_}};
  }

  [[nodiscard]] auto operator()(Box<ThisExpr> const& expr) -> Object {
    return (*this)(VariableExpr{expr->keyword_});
  }

  [[nodiscard]] auto operator()(Box<VariableExpr> const& expr) -> Object {
    if (auto const found{resolution_.find(expr->name_)};
        found != resolution_.end()) {
      std::size_t const distance = found->second;
      return environment_->get_at(expr->name_.lexeme_, distance);
    } else {
      throw Error{expr->name_.line_, expr->name_.lexeme_ + " is not defined"};
    }
  }

  [[nodiscard]] auto operator()(Box<AssignmentExpr> const& expr) -> Object {
    Object const value{std::visit(*this, expr->value_)};
    if (auto const found{resolution_.find(expr->name_)};
        found != resolution_.end()) {
      std::size_t const distance{found->second};
      environment_->assign_at(expr->name_.lexeme_, value, distance);
    } else {
      throw Error{expr->name_.line_, expr->name_.lexeme_ + " is not defined"};
    }

    return value;
  }

  [[nodiscard]] auto operator()(Box<BinaryExpr> const& expr) -> Object {
    Object const left{std::visit(*this, expr->left_)};
    Object const right{std::visit(*this, expr->right_)};

    Token const& op{expr->op_};
    TokenType const& op_type{op.type_};

    if (op_type == TokenType::MINUS) {
      check_number_operand(op, left, right);
      return std::get<double>(left) - std::get<double>(right);
    }
    if (op_type == TokenType::SLASH) {
      check_number_operand(op, left, right);
      return std::get<double>(left) / std::get<double>(right);
    }
    if (op_type == TokenType::STAR) {
      check_number_operand(op, left, right);
      return std::get<double>(left) * std::get<double>(right);
    }
    if (op_type == TokenType::PLUS) {
      if (std::holds_alternative<double>(left) &&
          std::holds_alternative<double>(right)) {
        return std::get<double>(left) + std::get<double>(right);
      }
      if (std::holds_alternative<std::string>(left) &&
          std::holds_alternative<std::string>(right)) {
        return std::get<std::string>(left) + std::get<std::string>(right);
      }
      throw Error{op.line_, "Operands must be two numbers or two strings."};
    }
    if (op_type == TokenType::GREATER) {
      check_number_operand(op, left, right);
      return std::get<double>(left) > std::get<double>(right);
    }
    if (op_type == TokenType::GREATER_EQUAL) {
      check_number_operand(op, left, right);
      return std::get<double>(left) >= std::get<double>(right);
    }
    if (op_type == TokenType::LESS) {
      check_number_operand(op, left, right);
      return std::get<double>(left) < std::get<double>(right);
    }
    if (op_type == TokenType::LESS_EQUAL) {
      check_number_operand(op, left, right);
      return std::get<double>(left) <= std::get<double>(right);
    }
    try {
      if (op_type == TokenType::BANG_EQUAL) {
        return !is_equal(left, right);
      }
      if (op_type == TokenType::EQUAL_EQUAL) {
        return is_equal(left, right);
      }
    } catch (NotComparableError const& e) {
      throw Error{
          op.line_,
          "Can only compare booleans, strings, and numbers for equality."};
    }
    // Unreachable
    return std::monostate{};
  }

  [[nodiscard]] auto operator()(Box<CallExpr> const& expr) -> Object {
    Object callee{std::visit(*this, expr->callee_)};

    std::vector<Object> args{};
    for (auto const& arg : expr->arguments_) {
      args.push_back(std::visit(*this, arg));
    }

    try {
      std::size_t const arity{std::visit(Arity{}, callee)};
      if (args.size() != arity) {
        throw Error{expr->paren_.line_, "Expected " + std::to_string(arity) +
                                            " arguments but got " +
                                            std::to_string(args.size()) + "."};
      }
      return std::visit(Call{environment_, resolution_, args}, callee);
    } catch (UncallableError const& e) {
      throw Error{expr->paren_.line_, "Can only call functions and classes."};
    }
  }

  [[nodiscard]] auto operator()(Box<GetExpr> const& expr) -> Object {
    Object const obj{std::visit(*this, expr->object_)};

    if (Arc<LoxInstance> const* const instance{
            std::get_if<Arc<LoxInstance>>(&obj)}) {
      return get(*instance, expr->name_);
    }

    throw Error{expr->name_.line_, "Only instances have properties."};
  }

  [[nodiscard]] auto operator()(Box<GroupingExpr> const& expr) -> Object {
    return std::visit(*this, expr->expression_);
  }

  [[nodiscard]] auto operator()(Box<LogicalExpr> const& expr) -> Object {
    Object const left{std::visit(*this, expr->left_)};

    if (expr->op_.type_ == TokenType::OR) {
      if (is_truthy(left)) {
        return left;
      }
    } else {
      if (!is_truthy(left)) {
        return left;
      }
    }
    return std::visit(*this, expr->right_);
  }

  [[nodiscard]] auto operator()(Box<SetExpr> const& expr) -> Object {
    Object obj{std::visit(*this, expr->object_)};

    if (auto const instance{std::get_if<Arc<LoxInstance>>(&obj)}) {
      Object const value{std::visit(*this, expr->value_)};
      set(*instance, expr->name_, value);

      return value;
    }

    throw Error{expr->name_.line_, "Only instances have properties."};
  }
  [[nodiscard]] auto operator()(Box<UnaryExpr> const& expr) -> Object {
    Object const right{std::visit(*this, expr->right_)};

    Token const& op{expr->op_};
    TokenType const& op_type{op.type_};

    if (op_type == TokenType::MINUS) {
      check_number_operand(op, right);
      return -std::get<double>(right);
    }

    if (op_type == TokenType::BANG) {
      return !is_truthy(right);
    }

    return std::monostate{};
  }
};

struct StatementExecutor {
  Arc<Environment> environment_;
  std::unordered_map<Token, std::size_t> const& resolution_;

  auto operator()(std::monostate) -> void {}

  auto operator()(Box<ExpressionStmt> const& stmt) -> void {
    static_cast<void>(std::visit(ExpressionEvaluator{environment_, resolution_},
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

    throw Return{value};
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
    if (is_truthy(std::visit(ExpressionEvaluator{environment_, resolution_},
                             stmt->condition_))) {
      std::visit(*this, stmt->then_branch_);
    } else {
      std::visit(*this, stmt->else_branch_);
    }
  }

  auto operator()(Box<WhileStmt> const& stmt) -> void {
    while (is_truthy(std::visit(ExpressionEvaluator{environment_, resolution_},
                                stmt->condition_))) {
      std::visit(*this, stmt->body_);
    }
  }
};

auto make_preamble_environment() -> Arc<Environment> {
  auto env{Arc{Environment{}}};

  for (auto const& name : Builtins::builtins()) {
    env->define(name.first, name.second);
  }

  return env;
}

auto interpret(std::vector<Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution,
               Arc<Environment> const& environment) -> void {
  for (auto const& stmt : statements) {
    std::visit(StatementExecutor{environment, resolution}, stmt);
  }
}

auto interpret(std::vector<Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution)
    -> void {
  Arc<Environment> const preamble_environment{make_preamble_environment()};

  interpret(statements, resolution, preamble_environment);
}

}  // namespace LOX::Interpreter
