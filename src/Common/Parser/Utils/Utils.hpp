#ifndef LOX_TREEWALK_PARSER_UTILS
#define LOX_TREEWALK_PARSER_UTILS

#include <vector>

#include "../../../Common/Parser/Cursor/Cursor.hpp"
#include "../../../Common/Parser/Error/Error.hpp"
#include "../../../Common/Types/Tokens/TokenTypes.hpp"

namespace LOX::Common::Parser::Utils {

template <typename T, typename F>
auto parse_parenthesized_list(Cursor& cursor, F const& f) -> std::vector<T> {
  cursor.consume(Types::Tokens::TokenType::LEFT_PAREN,
                 "Expect '(' before list.");

  std::vector<T> list{};

  if (!cursor.match(Types::Tokens::TokenType::RIGHT_PAREN)) {
    list.push_back(f(cursor));
    while (cursor.match(Types::Tokens::TokenType::COMMA)) {
      cursor.take();
      list.push_back(f(cursor));
    }
  }

  if (list.size() >= 255) {
    throw Common::Parser::error(cursor.peek(),
                                "Can't have more than 255 constituents.");
  }

  cursor.consume(Types::Tokens::TokenType::RIGHT_PAREN,
                 "Expect ')' after list.");

  return list;
}

}  // namespace LOX::Common::Parser::Utils
#endif
