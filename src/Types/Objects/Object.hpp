#ifndef LOX_TYPES_OBJECT
#define LOX_TYPES_OBJECT

#include <string>
#include <variant>

#include "../../Utils/Arc.hpp"
#include "../../Utils/Arcdyn.hpp"
#include "../../Utils/Box.hpp"

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

}  // namespace LOX::Types::Objects

#endif
