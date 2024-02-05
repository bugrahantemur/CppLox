#ifndef LOX_INTERPRETER_STATEMENTS_PUT
#define LOX_INTERPRETER_STATEMENTS_PUT

#include <string>
#include <variant>

#include "../../../Types/Objects/Class.hpp"
#include "../../../Types/Objects/Function.hpp"
#include "../../../Types/Objects/Object.hpp"

namespace LOX::Interpreter::Statements {

using namespace Types::Objects;

template <typename OStream>
struct Put {
  Put(OStream& out) : out{out} {}

  auto operator()(std::monostate) -> OStream& { return put("nil"); }

  auto operator()(std::string const& str) -> OStream& { return put(str); }

  auto operator()(double const number) -> OStream& { return put(number); }

  auto operator()(bool const b) -> OStream& {
    return b ? put("true") : put("false");
  }

  auto operator()(Box<LoxFunction> const& func) -> OStream& {
    return put("<fn " + func->declaration_.name.lexeme + ">");
  }

  auto operator()(Box<LoxClass> const& klass) -> OStream& {
    return put("<class " + klass->name + ">");
  }

  auto operator()(Arc<LoxInstance> const& instance) -> OStream& {
    return put("<instance of " + instance->class_.name + ">");
  }

  auto operator()(ArcDyn<Builtins::FunctionInterface> const& func) -> OStream& {
    return put("<builtin-fn " + func->to_string() + ">");
  }

 private:
  template <typename T>
  auto put(T const& obj) -> OStream& {
    out << obj << '\n';
    return out;
  }

  OStream& out;
};

}  // namespace LOX::Interpreter::Statements

#endif
