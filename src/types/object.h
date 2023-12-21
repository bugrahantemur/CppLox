#ifndef LOX_OBJECT
#define LOX_OBJECT
#include <string>
#include <variant>

using Object = std::variant<std::monostate, bool, double, std::string>;

#endif
