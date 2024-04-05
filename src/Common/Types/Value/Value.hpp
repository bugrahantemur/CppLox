#ifndef LOX_COMMON_TYPES_VALUE
#define LOX_COMMON_TYPES_VALUE

#include <string>
#include <variant>

namespace LOX::Common::Types {

using Value = std::variant<std::monostate, bool, double, std::string>;

}

#endif
