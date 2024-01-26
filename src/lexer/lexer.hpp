#ifndef LOX_SCANNER
#define LOX_SCANNER

#include <string>
#include <vector>

#include "../syntax/token.hpp"

namespace LOX::Lexer {

[[nodiscard]] auto scan(std::string const& contents) -> std::vector<Token>;

}  // namespace LOX::Lexer

#endif
