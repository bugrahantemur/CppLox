#ifndef LOX_PARSER
#define LOX_PARSER

#include <vector>

#include "statement.h"
#include "token.h"
namespace Parser {
auto parse(std::vector<Token const> const& tokens) -> std::vector<Statement>;
}  // namespace Parser

#endif
