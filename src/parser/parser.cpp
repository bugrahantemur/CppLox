#include "./parser.hpp"

#include <vector>

#include "../Token/Token.hpp"
#include "../syntax_types/statement.hpp"
#include "./cursor.hpp"
#include "./statements.hpp"

namespace LOX::Parser {

auto parse(std::vector<Token> const& tokens) -> std::vector<Statement> {
  Cursor cursor(tokens);

  std::vector<Statement> statements{};

  while (!cursor.is_at_end()) {
    statements.push_back(Statements::declaration(cursor));
  }

  return statements;
}
}  // namespace LOX::Parser
