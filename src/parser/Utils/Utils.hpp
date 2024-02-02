#ifndef LOX_PARSER_UTILS
#define LOX_PARSER_UTILS

#include <vector>

#include "../Cursor/Cursor.hpp"
#include "../Error/Error.hpp"

namespace LOX::Parser::Utils {

template <typename T, typename F>
auto parse_parenthesized_list(Cursor& cursor, F const& f) -> std::vector<T> {
  cursor.consume(TokenType::LEFT_PAREN, "Expect '(' before list.");

  std::vector<T> list{};

  if (!cursor.match(TokenType::RIGHT_PAREN)) {
    list.push_back(f(cursor));
    while (cursor.match(TokenType::COMMA)) {
      cursor.take();
      list.push_back(f(cursor));
    }
  }

  if (list.size() >= 255) {
    throw error(cursor.peek(), "Can't have more than 255 constituents.");
  }

  cursor.consume(TokenType::RIGHT_PAREN, "Expect ')' after list.");

  return list;
}

}  // namespace LOX::Parser::Utils
#endif
