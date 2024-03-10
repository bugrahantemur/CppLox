#include "./Parser.hpp"

#include <vector>

#include "../../Common/Types/Tokens/Token.hpp"
#include "../Types/Syntax/Statement.hpp"
#include "./Cursor/Cursor.hpp"
#include "./Statements/Statements.hpp"

namespace LOX::TreeWalk::Parser {

auto parse(std::vector<LOX::Types::Token> const& tokens)
    -> std::vector<Types::Syntax::Statements::Statement> {
  Cursor cursor(tokens);

  std::vector<Types::Syntax::Statements::Statement> statements{};

  while (!cursor.is_at_end()) {
    statements.push_back(Statements::declaration(cursor));
  }

  return statements;
}
}  // namespace LOX::TreeWalk::Parser
