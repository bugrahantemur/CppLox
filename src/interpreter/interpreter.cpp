#include "./interpreter.hpp"

#include <chrono>
#include <concepts>
#include <iostream>
#include <string>
#include <variant>

#include "../types/class.hpp"
#include "../types/expression.hpp"
#include "../types/function.hpp"
#include "../types/object.hpp"
#include "../types/statement.hpp"
#include "../types/token.hpp"
#include "../utils/box.hpp"
#include "./error.hpp"

namespace LOX {
struct BuiltinFunction {
  virtual auto arity() const -> std::size_t = 0;
  virtual auto to_string() const -> std::string = 0;
  virtual auto operator()(std::vector<Object> const& args) const -> Object = 0;
};

struct Clock : public BuiltinFunction {
  auto arity() const -> std::size_t final { return 0; }
  auto to_string() const -> std::string final { return "Clock"; };
  auto operator()(std::vector<Object> const& args) const -> Object final {
    static_cast<void>(args);

    return static_cast<double>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  };
};
}  // namespace LOX

namespace LOX::Interpreter {

struct Return {
  Object value_;
};

template <typename... Objects>
auto check_number_operand(Token const& token, Objects... operands) -> void {
  std::array const ops{operands...};
  if (std::all_of(std::begin(ops), std::end(ops), [](Object const& obj) {
        return !std::holds_alternative<double>(obj);
      })) {
    throw Error{token.line_, sizeof...(operands) > 1
                                 ? "Operands must be numbers."
                                 : "Operand must be a number."};
  }
}

struct Equality {
  auto operator()(std::monostate, std::monostate) -> bool { return true; }
  auto operator()(bool left, bool right) -> bool { return left == right; }
  auto operator()(double left, double right) -> bool { return left == right; }
  auto operator()(std::string const& left, std::string const& right) -> bool {
    return left == right;
  }

  template <typename T, typename U>
  auto operator()(T const& left, U const& right) -> bool {
    throw Error{0, "Can only compare booleans, strings, and numbers"};
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

  auto operator()(std::shared_ptr<LoxInstance> const& instance) -> OStream& {
    return put("<instance of " + instance->class_.name_ + ">");
  }

  auto operator()(std::shared_ptr<BuiltinFunction> const& func) -> OStream& {
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

  auto operator()(std::shared_ptr<BuiltinFunction> const& func) -> std::size_t {
    return func->arity();
  }

  template <typename T>
  auto operator()(T const& t) -> std::size_t {
    throw UncallableError{};
  }
};

struct Call {
  auto operator()(Box<LoxFunction>& func) -> Object {
    auto const env{std::make_shared<Environment>(func->closure_)};

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
    auto const instance{std::make_shared<LoxInstance>(LoxInstance{*klass, {}})};

    if (auto const initializer{klass->methods_.find("init")};
        initializer != klass->methods_.end()) {
      auto const env{
          std::make_shared<Environment>(initializer->second.closure_)};
      env->define("this", instance);
      auto const init{
          Box{LoxFunction{initializer->second.declaration_, env, true}}};
      (*this)(init);
    }

    return instance;
  }

  auto operator()(std::shared_ptr<BuiltinFunction> const& func) -> Object {
    return (*func)(args_);
  }

  template <typename T>
  auto operator()(T const& t) -> Object {
    throw UncallableError{};
  }

  std::shared_ptr<Environment> environment_;
  std::unordered_map<Token, std::size_t> const& resolution_;
  std::vector<Object> const& args_;
};

struct ExpressionEvaluator {
  std::shared_ptr<Environment> environment_;
  std::unordered_map<Token, std::size_t> const& resolution_;

  [[nodiscard]] auto operator()(std::monostate) -> Object {
    return std::monostate{};
  }

  [[nodiscard]] auto operator()(Box<LiteralExpression> const& expr) -> Object {
    return std::visit([](auto const& value) -> Object { return value; },
                      expr->value_);
  }

  [[nodiscard]] auto operator()(Box<SuperExpression> const& expr) -> Object {
    std::size_t const distance{resolution_.at(expr->keyword_)};

    auto const superclass{
        std::get<Box<LoxClass>>(environment_->get_at("super", distance))};

    auto const instance{std::get<std::shared_ptr<LoxInstance>>(
        environment_->get_at("this", distance - 1))};

    std::optional<LoxFunction> const method{
        superclass->find_method(expr->method_.lexeme_)};

    if (!method) {
      throw Error{expr->method_.line_,
                  "Undefined property '" + expr->method_.lexeme_ + "'"};
    }

    auto const env{std::make_shared<Environment>(method.value().closure_)};
    env->define("this", instance);
    return LoxFunction{method.value().declaration_, env,
                       method.value().is_initializer_};
  }

  [[nodiscard]] auto operator()(Box<ThisExpression> const& expr) -> Object {
    return (*this)(VariableExpression{expr->keyword_});
  }

  [[nodiscard]] auto operator()(Box<VariableExpression> const& expr) -> Object {
    if (auto const found{resolution_.find(expr->name_)};
        found != resolution_.end()) {
      std::size_t const distance = found->second;
      return environment_->get_at(expr->name_.lexeme_, distance);
    } else {
      throw Error{expr->name_.line_, expr->name_.lexeme_ + " is not defined"};
    }
  }

  [[nodiscard]] auto operator()(Box<AssignmentExpression> const& expr)
      -> Object {
    Object const value{std::visit(*this, expr->value_)};
    if (auto const found{resolution_.find(expr->name_)};
        found != resolution_.end()) {
      std::size_t const distance = found->second;
      environment_->assign_at(expr->name_.lexeme_, value, distance);
    } else {
      throw Error{expr->name_.line_, expr->name_.lexeme_ + " is not defined"};
    }

    return value;
  }

  [[nodiscard]] auto operator()(Box<BinaryExpression> const& expr) -> Object {
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
    if (op_type == TokenType::BANG_EQUAL) {
      return !is_equal(left, right);
    }
    if (op_type == TokenType::EQUAL_EQUAL) {
      return is_equal(left, right);
    }
    // Unreachable
    return std::monostate{};
  }

  [[nodiscard]] auto operator()(Box<CallExpression> const& expr) -> Object {
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

  [[nodiscard]] auto operator()(Box<GetExpression> const& expr) -> Object {
    Object const obj{std::visit(*this, expr->object_)};

    if (auto const instance{std::get_if<std::shared_ptr<LoxInstance>>(&obj)}) {
      return get(*instance, expr->name_);
    }

    throw Error{expr->name_.line_, "Only instances have properties."};
  }

  [[nodiscard]] auto operator()(Box<GroupingExpression> const& expr) -> Object {
    return std::visit(*this, expr->expression_);
  }

  [[nodiscard]] auto operator()(Box<LogicalExpression> const& expr) -> Object {
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

  [[nodiscard]] auto operator()(Box<SetExpression> const& expr) -> Object {
    Object obj{std::visit(*this, expr->object_)};

    if (auto const instance{std::get_if<std::shared_ptr<LoxInstance>>(&obj)}) {
      Object const value{std::visit(*this, expr->value_)};
      set(*instance, expr->name_, value);

      return value;
    }

    throw Error{expr->name_.line_, "Only instances have properties."};
  }
  [[nodiscard]] auto operator()(Box<UnaryExpression> const& expr) -> Object {
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
  std::shared_ptr<Environment> environment_;
  std::unordered_map<Token, std::size_t> const& resolution_;

  auto operator()(std::monostate) -> void {}

  auto operator()(Box<ExpressionStatement> const& stmt) -> void {
    static_cast<void>(std::visit(ExpressionEvaluator{environment_, resolution_},
                                 stmt->expression_));
  }

  auto operator()(Box<PrintStatement> const& stmt) -> void {
    Object const value{std::visit(
        ExpressionEvaluator{environment_, resolution_}, stmt->expression_)};
    std::visit(Put{std::cout}, value);
  }

  auto operator()(Box<ReturnStatement> const& stmt) -> void {
    Object const value{std::visit(
        ExpressionEvaluator{environment_, resolution_}, stmt->value_)};

    throw Return{value};
  }

  auto operator()(Box<VariableStatement> const& stmt) -> void {
    Object const value{std::visit(
        ExpressionEvaluator{environment_, resolution_}, stmt->initializer_)};

    environment_->define(stmt->name_.lexeme_, value);
  }

  auto operator()(Box<BlockStatement> const& stmt) -> void {
    // Create new environment with the current environment as its
    // enclosing environment
    auto const env{std::make_shared<Environment>(environment_)};

    // Execute statements in the block with the new environment
    for (Statement const& statement : stmt->statements_) {
      std::visit(StatementExecutor{env, resolution_}, statement);
    }
  }

  auto operator()(Box<FunctionStatement> const& stmt) -> void {
    environment_->define(stmt->name_.lexeme_,
                         LoxFunction{*stmt, environment_, false});
  }

  auto operator()(Box<ClassStatement> const& stmt) -> void {
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
      environment_ = std::make_shared<Environment>(environment_);
      environment_->define("super", superclass.value());
    }

    std::unordered_map<std::string, LoxFunction> class_methods;
    for (Box<FunctionStatement> const& method : stmt->methods_) {
      class_methods[method->name_.lexeme_] =
          LoxFunction{*method, environment_, method->name_.lexeme_ == "init"};
    }

    if (superclass) {
      environment_ = environment_->enclosing_;
    }

    environment_->assign(
        stmt->name_.lexeme_,
        LoxClass{stmt->name_.lexeme_, superclass, class_methods});
  }

  auto operator()(Box<IfStatement> const& stmt) -> void {
    if (is_truthy(std::visit(ExpressionEvaluator{environment_, resolution_},
                             stmt->condition_))) {
      std::visit(*this, stmt->then_branch_);
    } else {
      std::visit(*this, stmt->else_branch_);
    }
  }

  auto operator()(Box<WhileStatement> const& stmt) -> void {
    while (is_truthy(std::visit(ExpressionEvaluator{environment_, resolution_},
                                stmt->condition_))) {
      std::visit(*this, stmt->body_);
    }
  }
};

auto interpret(std::vector<Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution,
               std::shared_ptr<Environment> const& environment) -> void {
  for (auto const& stmt : statements) {
    std::visit(StatementExecutor{environment, resolution}, stmt);
  }
}

auto interpret(std::vector<Statement> const& statements,
               std::unordered_map<Token, std::size_t> const& resolution)
    -> void {
  auto env{std::make_shared<Environment>()};
  env->define("clock", std::make_shared<Clock>());
  interpret(statements, resolution, env);
}

}  // namespace LOX::Interpreter
