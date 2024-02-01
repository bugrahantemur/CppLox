#ifndef LOX_TYPES_OBJECT
#define LOX_TYPES_OBJECT

#include <string>
#include <variant>

#include "../Utils/arc.hpp"
#include "../Utils/arcdyn.hpp"
#include "../Utils/box.hpp"

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
                            Arc<Objects::LoxInstance>,
                            ArcDyn<Builtins::FunctionInterface>>;

}  // namespace LOX

#endif
