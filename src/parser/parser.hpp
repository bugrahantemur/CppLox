#ifndef LOX_PARSER
#define LOX_PARSER

#include <vector>

#include "../Token/Token.hpp"
#include "../syntax_types/statement.hpp"

namespace LOX::Parser {
auto parse(std::vector<Token> const& tokens) -> std::vector<Statement>;
}  // namespace LOX::Parser

#endif
