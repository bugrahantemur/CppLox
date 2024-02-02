#include "./Parser.hpp"

#include <vector>

#include "../Types/Syntax/Statement.hpp"
#include "../Types/Token/Token.hpp"
#include "./Cursor/Cursor.hpp"
#include "./Statements/Statements.hpp"

namespace LOX::Parser {

auto parse(std::vector<Token> const& tokens)
    -> std::vector<Types::Syntax::Statement> {
  Cursor cursor(tokens);

  std::vector<Types::Syntax::Statement> statements{};

  while (!cursor.is_at_end()) {
    statements.push_back(Statements::declaration(cursor));
  }

  return statements;
}
}  // namespace LOX::Parser
