#ifndef LOX_PARSER
#define LOX_PARSER

#include <vector>

#include "../Types/Syntax/Statement.hpp"
#include "../Types/Token/Token.hpp"

namespace LOX::Parser {
auto parse(std::vector<Token> const& tokens)
    -> std::vector<Types::Syntax::Statements::Statement>;
}  // namespace LOX::Parser

#endif
