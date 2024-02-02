#include "Scanner.hpp"

#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "../Types/Token/Token.hpp"
#include "./Cursor.hpp"
#include "./Error/Error.hpp"
#include "./Tokens.hpp"

namespace LOX::Scanner {

[[nodiscard]] auto scan_token(Cursor& cursor) -> std::optional<Token> {
  char const c{cursor.take()};

  // whitespace
  if (c == ' ' || c == '\r' || c == '\t') {
    return handle_whitespace(cursor);
  }
  // newline
  if (c == '\n') {
    return handle_newline(cursor);
  }
  // slash
  if (c == '/') {
    return handle_slash(cursor);
  }
  // string literal
  if (c == '"') {
    return handle_string_literal(cursor);
  }
  // number literal
  if (std::isdigit(c)) {
    return handle_number_literal(cursor);
  }
  // identifier
  if (std::isalpha(c) || c == '_') {
    return handle_identifier(cursor);
  }
  // single or double character tokens
  if (auto const sp_char_token = build_special_character_token(cursor, c)) {
    return sp_char_token.value();
  }

  error(cursor.at_line(), "Unexpected character '" + std::string{c} + "'");
  return std::nullopt;
}
[[nodiscard]] auto scan(std::string const& contents) -> std::vector<Token> {
  std::vector<Token> tokens;
  Cursor cursor(contents);

  while (!cursor.is_at_end()) {
    cursor.advance_word();
    if (std::optional<Token> const token{scan_token(cursor)}) {
      tokens.push_back(token.value());
    }
  }

  tokens.emplace_back(Token{TokenType::EOFF, "", std::monostate{},
                            cursor.at_line(), cursor.index()});

  return tokens;
}
}  // namespace LOX::Scanner
