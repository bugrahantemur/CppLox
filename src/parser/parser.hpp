#ifndef LOX_PARSER
#define LOX_PARSER

#include <vector>

#include "../syntax/statement.hpp"
#include "../syntax/token.hpp"

namespace LOX::Parser {
auto parse(std::vector<Token> const& tokens) -> std::vector<Statement>;
}  // namespace LOX::Parser

#endif
