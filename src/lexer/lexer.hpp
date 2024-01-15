#ifndef LOX_SCANNER
#define LOX_SCANNER

#include <string>
#include <vector>

#include "../types/token.hpp"
#include "../utils/error.hpp"

namespace LOX::Lexer {

[[nodiscard]] auto scan_tokens(std::string const& contents)
    -> std::vector<Token>;

}  // namespace LOX::Lexer

#endif
