#ifndef LOX_COMMON_SCANNER
#define LOX_COMMON_SCANNER

#include <string>
#include <vector>

#include "../Types/Tokens/Token.hpp"

namespace LOX::Common::Scanner {

[[nodiscard]] auto scan(std::string const& contents)
    -> std::vector<Types::Token>;

}  // namespace LOX::Common::Scanner

#endif
