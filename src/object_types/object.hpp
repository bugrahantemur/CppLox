#ifndef LOX_TYPES_OBJECT
#define LOX_TYPES_OBJECT

#include <memory>
#include <string>
#include <variant>

#include "../utils/box.hpp"

namespace LOX {

namespace Objects {
struct LoxFunction;
struct LoxClass;
struct LoxInstance;
}  // namespace Objects

namespace Builtins {
struct FunctionInterface;
}

using Object = std::variant<std::monostate, bool, double, std::string,
                            Box<Objects::LoxFunction>, Box<Objects::LoxClass>,
                            std::shared_ptr<Objects::LoxInstance>,
                            std::shared_ptr<Builtins::FunctionInterface>>;

}  // namespace LOX

#endif
