#ifndef LOX_TYPES_OBJECT
#define LOX_TYPES_OBJECT
#include <string>
#include <variant>

using Object = std::variant<std::monostate, bool, double, std::string>;

#endif
