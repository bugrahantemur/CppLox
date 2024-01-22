#ifndef LOX_BUILTINS_NAMES
#define LOX_BUILTINS_NAMES

#include <string>
#include <vector>

namespace LOX::Builtins {

inline auto names() -> std::vector<std::string> { return {"clock", "pi"}; }

}  // namespace LOX::Builtins

#endif
