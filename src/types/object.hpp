#ifndef LOX_TYPES_OBJECT
#define LOX_TYPES_OBJECT

#include <memory>
#include <string>
#include <variant>

#include "../utils/box.hpp"

namespace LOX {

struct LoxFunction;
struct LoxClass;
class LoxInstance;

namespace Builtins {
struct FunctionInterface;
}

using Object =
    std::variant<std::monostate, bool, double, std::string, Box<LoxFunction>,
                 Box<LoxClass>, std::shared_ptr<LoxInstance>,
                 std::shared_ptr<Builtins::FunctionInterface>>;

}  // namespace LOX

#endif
