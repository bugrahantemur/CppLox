#ifndef LOX_COMMON_UTILS_PUT
#define LOX_COMMON_UTILS_PUT

#include <string>
#include <variant>

#include "../../Types/Objects/Object.hpp"

namespace LOX::Common::Utils {

template <typename OStream>
struct Put {
  explicit Put(OStream& out) : out{out} {}

  auto operator()(std::monostate none) -> OStream& {
    (void)none;
    return put("nil");
  }

  auto operator()(std::string const& str) -> OStream& { return put(str); }

  auto operator()(double const number) -> OStream& { return put(number); }

  auto operator()(bool const val) -> OStream& {
    return val ? put("true") : put("false");
  }

  auto operator()(Box<Common::Types::Objects::LoxFunction> const& func)
      -> OStream& {
    return put("<fn " + func->declaration.name.lexeme + ">");
  }

  auto operator()(Box<Common::Types::Objects::LoxClass> const& klass)
      -> OStream& {
    return put("<class " + klass->name + ">");
  }

  auto operator()(Arc<Common::Types::Objects::LoxInstance> const& instance)
      -> OStream& {
    return put("<instance of " + instance->class_.name + ">");
  }

  auto operator()(
      ArcDyn<Common::Types::Objects::Builtins::FunctionInterface> const& func)
      -> OStream& {
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

}  // namespace LOX::Common::Utils

#endif
