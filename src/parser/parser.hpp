#ifndef LOX_PARSER
#define LOX_PARSER

#include <vector>

#include "../types/statement.hpp"
#include "../types/token.hpp"

namespace LOX::Parser {
auto parse(std::vector<Token> const& tokens) -> std::vector<Statement>;
}  // namespace LOX::Parser

#endif
