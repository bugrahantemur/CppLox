#ifndef LOX_SCANNER
#define LOX_SCANNER

#include <string>
#include <vector>

#include "../../Common/Types/Tokens/Token.hpp"

namespace LOX::Scanner {

[[nodiscard]] auto scan(std::string const& contents)
    -> std::vector<Types::Token>;

}  // namespace LOX::Scanner

#endif
