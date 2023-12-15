#ifndef LOX_PARSER
#define LOX_PARSER

#include <vector>

#include "expression.h"
#include "token.h"
namespace Parser {
auto parse(std::vector<Token const> const& tokens) -> Expression;
}  // namespace Parser

#endif
