#ifndef LOX_BUILTINS_FUNCTION_INTERFACE
#define LOX_BUILTINS_FUNCTION_INTERFACE

#include <string>
#include <vector>

#include "../objects/object.hpp"

namespace LOX::Builtins {

struct FunctionInterface {
  virtual auto arity() const -> std::size_t = 0;
  virtual auto to_string() const -> std::string = 0;
  virtual auto operator()(std::vector<Object> const& args) const -> Object = 0;
};

}  // namespace LOX::Builtins

#endif
