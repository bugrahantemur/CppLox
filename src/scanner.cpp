#include "scanner.h"

#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "token.h"
#include "utils/error.h"

namespace {

struct Cursor {
  std::string const& source;
  int start;
  int current;
  int line;
};

[[nodiscard]] auto is_word_char(char const c) -> bool {
  return std::isalnum(c) || c == '_';
}

[[nodiscard]] auto take(Cursor& cursor) -> char {
  return cursor.source.at(cursor.current++);
}

auto advance(Cursor& cursor) -> void { cursor.current++; }

[[nodiscard]] auto is_at_end(Cursor const& cursor) -> bool {
  return cursor.current >= cursor.source.size();
}

[[nodiscard]] auto peek(Cursor const& cursor) -> char {
  if (is_at_end(cursor)) return '\0';

  return cursor.source.at(cursor.current);
}

[[nodiscard]] auto peek_next(Cursor const& cursor) -> char {
  return peek(
      Cursor{cursor.source, cursor.start, cursor.current + 1, cursor.line});
}

[[nodiscard]] auto match(Cursor& cursor, char const expected) -> bool {
  if (is_at_end(cursor) || peek(cursor) != expected) {
    return false;
  };

  return true;
}

using Literal = std::variant<std::monostate, std::string, double>;
[[nodiscard]] auto make_token(Cursor const& cursor, TokenType token_type,
                              Literal const& literal) -> Token {
  auto const text =
      cursor.source.substr(cursor.start, cursor.current - cursor.start);
  return {token_type, text, literal, cursor.line};
}

[[nodiscard]] auto make_token(Cursor const& cursor, TokenType token_type)
    -> Token {
  return make_token(cursor, token_type, std::monostate{});
}

[[nodiscard]] auto handle_string_literal(Cursor& cursor) -> Token {
  while (peek(cursor) != '"' and !is_at_end(cursor)) {
    if (peek(cursor) == '\n') {
      cursor.line++;
    }
    advance(cursor);
  }

  if (is_at_end(cursor)) {
    auto const message = "Unterminated string literal";
    throw LoxError{cursor.line, message};
  }

  // Closing double quotes
  advance(cursor);

  auto const value =
      cursor.source.substr(cursor.start + 1, cursor.current - cursor.start - 2);
  return make_token(cursor, TokenType::STRING, value);
}

[[nodiscard]] auto handle_number_literal(Cursor& cursor) -> Token {
  while (std::isdigit(peek(cursor))) {
    advance(cursor);
  }

  // Look for a fractional part.
  if (peek(cursor) == '.' && std::isdigit(peek_next(cursor))) {
    // Consume the "."
    advance(cursor);

    while (std::isdigit(peek(cursor))) {
      advance(cursor);
    }
  }

  return make_token(cursor, TokenType::NUMBER,
                    std::stod(cursor.source.substr(
                        cursor.start, cursor.current - cursor.start)));
}

[[nodiscard]] auto handle_identifier(Cursor& cursor) -> Token {
  while (is_word_char(peek(cursor))) {
    advance(cursor);
  }

  std::string const& text =
      cursor.source.substr(cursor.start, cursor.current - cursor.start);
  // Text is either a reserved keyword, or a regular user-defined identifier
  auto const token_type =
      match_keyword_token_type(text).value_or(TokenType::IDENTIFIER);
  return make_token(cursor, token_type);
}

[[nodiscard]] auto build_single_or_double_character_token(Cursor& cursor,
                                                          char const c)
    -> std::optional<Token> {
  // For chars that definitely make a single char token
  auto const single = [&cursor](auto const token_type) {
    return make_token(cursor, token_type);
  };

  // For characters that may make a double char token when followed by '='
  auto const single_or_double = [&cursor](auto const with_eq,
                                          auto const without_eq) {
    auto const matched = match(cursor, '=');
    if (matched) {
      advance(cursor);
      return make_token(cursor, with_eq);
    }
    return make_token(cursor, without_eq);
  };

  switch (c) {
    case '(':
      return single(TokenType::LEFT_PAREN);
    case ')':
      return single(TokenType::RIGHT_PAREN);
    case '{':
      return single(TokenType::LEFT_BRACE);
    case '}':
      return single(TokenType::RIGHT_BRACE);
    case ',':
      return single(TokenType::COMMA);
    case '.':
      return single(TokenType::DOT);
    case '-':
      return single(TokenType::MINUS);
    case '+':
      return single(TokenType::PLUS);
    case ';':
      return single(TokenType::SEMICOLON);
    case '*':
      return single(TokenType::STAR);
    case '!':
      return single_or_double(TokenType::BANG_EQUAL, TokenType::BANG);
    case '=':
      return single_or_double(TokenType::EQUAL_EQUAL, TokenType::EQUAL);
    case '<':
      return single_or_double(TokenType::LESS_EQUAL, TokenType::LESS);
    case '>':
      return single_or_double(TokenType::GREATER_EQUAL, TokenType::GREATER);
    default:
      return std::nullopt;
  }
}

[[nodiscard]] auto handle_slash(Cursor& cursor) -> std::optional<Token> {
  // Comments start with a double slash.
  if (match(cursor, '/')) {
    // A comment goes until the end of the line.
    while (peek(cursor) != '\n' && !is_at_end(cursor)) {
      advance(cursor);
    }
    return std::nullopt;
  }

  // Not a comment
  return make_token(cursor, TokenType::SLASH);
}

auto handle_newline(Cursor& cursor) -> std::nullopt_t {
  cursor.line++;
  return std::nullopt;
}

auto handle_whitespace(Cursor& cursor) -> std::nullopt_t {
  return std::nullopt;
}

[[nodiscard]] auto scan_token(Cursor& cursor) -> std::optional<Token> {
  auto const c = take(cursor);

  // newline
  if (c == '\n') {
    return handle_newline(cursor);
  }
  // whitespace
  if (c == ' ' || c == '\r' || c == '\t') {
    return handle_whitespace(cursor);
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
  if (auto const sod_char_token =
          build_single_or_double_character_token(cursor, c)) {
    return sod_char_token.value();
  }

  auto const message = "Unexpected character '" + std::string(1, c) + "'";
  throw LoxError(cursor.line, message);
}
}  // namespace

namespace Scanner {
[[nodiscard]] auto scan_tokens(std::string const& contents)
    -> std::vector<Token> {
  std::vector<Token> tokens;
  Cursor cursor{contents, 0, 0, 1};

  while (!is_at_end(cursor)) {
    cursor.start = cursor.current;
    if (auto const token = scan_token(cursor)) {
      tokens.push_back(token.value());
      std::cout << token.value().to_string();
    }
  }

  tokens.emplace_back(TokenType::EOFF, "", std::monostate{}, cursor.line);

  return tokens;
}
}  // namespace Scanner
