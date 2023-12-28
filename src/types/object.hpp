#ifndef LOX_TYPES_OBJECT
#define LOX_TYPES_OBJECT

#include <string>
#include <variant>

#include "../utils/box.hpp"
#include "./function.hpp"

using Object =
    std::variant<std::monostate, bool, double, std::string, LoxFunction>;

#endif
