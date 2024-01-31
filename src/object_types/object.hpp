#ifndef LOX_TYPES_OBJECT
#define LOX_TYPES_OBJECT

#include <string>
#include <variant>

#include "../utils/arc.hpp"
#include "../utils/arcdyn.hpp"
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
                            Arc<Objects::LoxInstance>,
                            ArcDyn<Builtins::FunctionInterface>>;

}  // namespace LOX

#endif
