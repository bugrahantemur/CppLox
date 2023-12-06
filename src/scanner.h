#ifndef LOX_SCANNER
#define LOX_SCANNER

#include <string>
#include <vector>

#include "token.h"

namespace Scanner {
[[nodiscard]] auto scan_tokens(std::string const& contents)
    -> std::vector<Token const>;
}  // namespace Scanner

#endif
