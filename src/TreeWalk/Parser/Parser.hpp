#ifndef LOX_TREEWALK_PARSER
#define LOX_TREEWALK_PARSER

#include <vector>

#include "../../Common/Types/Tokens/Token.hpp"
#include "../Types/Syntax/Statement.hpp"

namespace LOX::TreeWalk::Parser {
auto parse(std::vector<LOX::Common::Types::Token> const& tokens)
    -> std::vector<Types::Syntax::Statements::Statement>;
}  // namespace LOX::TreeWalk::Parser

#endif
