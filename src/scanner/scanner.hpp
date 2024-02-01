#ifndef LOX_SCANNER
#define LOX_SCANNER

#include <string>
#include <vector>

#include "../Types/Token/Token.hpp"

namespace LOX::Scanner {

[[nodiscard]] auto scan(std::string const& contents) -> std::vector<Token>;

}  // namespace LOX::Scanner

#endif
