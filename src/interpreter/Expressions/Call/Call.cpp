#include "./Call.hpp"

#include <exception>

#include "../../../Types/Object/Builtins/FunctionInterface.hpp"
#include "../../../Types/Object/Class.hpp"
#include "../../../Types/Object/Function.hpp"
#include "../../../Types/Object/Object.hpp"
#include "../../../Utils/Arcdyn.hpp"
#include "../../../Utils/Box.hpp"
#include "../../Utils/Return/Return.hpp"

namespace LOX::Interpreter::Expressions {

using namespace Types::Objects;

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

  auto operator()(Box<LoxFunction> const& func) -> Object {
    auto env{Arc{Environment{func->closure_}}};

    std::size_t const arity{Arity{}(func)};

    for (std::size_t i = 0; i < arity; ++i) {
      env->define(func->declaration_.params_.at(i).lexeme_, args_.at(i));
    }

    try {
      Interpreter::interpret(func->declaration_.body_, resolution_, env);
    } catch (Utils::Return const& ret) {
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

auto call(Arc<Environment> environment,
          std::unordered_map<Token, std::size_t> const& resolution,
          std::vector<Object> const& args, Object const& callee) -> Object {
  return std::visit(Call{environment, resolution, args}, callee);
}

auto arity_of(Object const& callee) -> std::size_t {
  return std::visit(Arity{}, callee);
}

}  // namespace LOX::Interpreter::Expressions
