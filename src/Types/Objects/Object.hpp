#ifndef LOX_TYPES_OBJECT
#define LOX_TYPES_OBJECT

#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

#include "../../Utils/Arc.hpp"
#include "../../Utils/Arcdyn.hpp"
#include "../../Utils/Box.hpp"
#include "../Syntax/Statement.hpp"

namespace LOX::Types {
struct Environment;
}

namespace LOX::Types::Objects {

struct LoxFunction;
struct LoxClass;
struct LoxInstance;

namespace Builtins {
struct FunctionInterface;
}

using Object = std::variant<std::monostate, bool, double, std::string,
                            Box<Objects::LoxFunction>, Box<Objects::LoxClass>,
                            Arc<Objects::LoxInstance>,
                            ArcDyn<Objects::Builtins::FunctionInterface>>;

struct LoxFunction {
  Syntax::Statements::FunctionStmt declaration_;
  Arc<Environment> closure_;
  bool is_initializer;
};

struct LoxClass {
  std::string name;
  std::optional<Box<LoxClass>> super_class;
  std::unordered_map<std::string, LoxFunction> methods;
};

struct LoxInstance {
  LoxClass class_;
  std::unordered_map<std::string, Object> fields;
};

namespace Builtins {
struct FunctionInterface {
  virtual auto arity() const -> std::size_t = 0;
  virtual auto to_string() const -> std::string = 0;
  virtual auto operator()(std::vector<Object> const& args) const -> Object = 0;
};

}  // namespace Builtins

}  // namespace LOX::Types::Objects

#endif
