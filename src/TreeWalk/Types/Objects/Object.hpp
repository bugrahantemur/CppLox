#ifndef LOX_TREEWALK_TYPES_OBJECT
#define LOX_TREEWALK_TYPES_OBJECT

#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

#include "../../../../submodules/RustyPtr/include/RustyPtr/Arc.hpp"
#include "../../../../submodules/RustyPtr/include/RustyPtr/ArcDyn.hpp"
#include "../../../../submodules/RustyPtr/include/RustyPtr/Box.hpp"
#include "../../../Common/Types/Syntax/Statement.hpp"

namespace LOX::TreeWalk::Types {
struct Environment;
}

namespace LOX::TreeWalk::Types::Objects {

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
  Common::Types::Syntax::Statements::FunctionStmt declaration;
  Arc<Environment> closure;
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
  FunctionInterface() = default;

  virtual ~FunctionInterface() = default;

  FunctionInterface(FunctionInterface const& other) = default;
  auto operator=(FunctionInterface const& other)
      -> FunctionInterface& = default;

  FunctionInterface(FunctionInterface&& other) = default;
  auto operator=(FunctionInterface&& other) -> FunctionInterface& = default;

  [[nodiscard]] virtual auto arity() const -> std::size_t = 0;

  [[nodiscard]] virtual auto to_string() const -> std::string = 0;

  virtual auto operator()(Arc<Types::Environment> const& environment,
                          std::unordered_map<LOX::Common::Types::Token,
                                             std::size_t> const& resolution,
                          std::vector<Object> const& args) const -> Object = 0;
};

}  // namespace Builtins

}  // namespace LOX::TreeWalk::Types::Objects

#endif
