#include "./Parser.hpp"

#include <vector>

#include "../Parser/Cursor/Cursor.hpp"
#include "../Types/Syntax/Statement.hpp"
#include "../Types/Tokens/Token.hpp"
#include "./Statements/Statements.hpp"

namespace LOX::Common::Parser {

auto parse(std::vector<Types::Token> const& tokens)
    -> std::vector<Types::Syntax::Statements::Statement> {
  Common::Parser::Cursor cursor(tokens);

  std::vector<Types::Syntax::Statements::Statement> statements{};

  while (!cursor.is_at_end()) {
    statements.push_back(Statements::declaration(cursor));
  }

  return statements;
}
}  // namespace LOX::Common::Parser
