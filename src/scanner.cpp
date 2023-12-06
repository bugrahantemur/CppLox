#include "scanner.h"

#include <cassert>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

#include "token.h"
#include "utils/error.h"

namespace {

[[nodiscard]] auto is_word_char(char const c) -> bool {
  return std::isalnum(c) || c == '_';
}

class Cursor {
 public:
  Cursor(std::string const& source)
      : source_(source), start_(0), current_(0), line_(1) {}

  auto advance() -> void { current_++; }

  auto down() -> void { line_++; }

  auto advance_word() -> void { start_ = current_; }

  [[nodiscard]] auto peek(std::size_t forward = 0) const -> char {
    if (is_at_end()) return '\0';

    assert(current_ + forward < source_.size());

    return source_.at(current_ + forward);
  }

  [[nodiscard]] auto take() -> char {
    auto const tmp = peek();
    advance();
    return tmp;
  }

  [[nodiscard]] auto peek_word() const -> std::string {
    return source_.substr(start_, current_ - start_);
  }

  [[nodiscard]] auto at_line() const -> std::size_t { return line_; }

  [[nodiscard]] auto is_at_end() const -> bool {
    return current_ >= source_.size();
  }

  [[nodiscard]] auto match(char const expected) const -> bool {
    if (is_at_end() || peek() != expected) {
      return false;
    };

    return true;
  }

 private:
  std::string const& source_;
  std::size_t start_;
  std::size_t current_;
  std::size_t line_;
};

[[nodiscard]] auto make_token(Cursor& cursor, TokenType token_type,
                              Token::Literal const& literal = std::monostate{})
    -> Token {
  auto const text = cursor.peek_word();
  return Token{token_type, text, literal, cursor.at_line()};
}

[[nodiscard]] auto handle_string_literal(Cursor& cursor) -> Token {
  while (cursor.peek() != '"' and !cursor.is_at_end()) {
    if (cursor.peek() == '\n') {
      cursor.down();
    }
    cursor.advance();
  }

  if (cursor.is_at_end()) {
    auto const message = "Unterminated string literal";
    throw LoxError{cursor.at_line(), message};
  }

  // Closing double quotes
  cursor.advance();

  auto const text = cursor.peek_word();

  // Trim the surrounding quotes
  auto const value = text.substr(1, text.size() - 2);

  return make_token(cursor, TokenType::STRING, value);
}

[[nodiscard]] auto handle_number_literal(Cursor& cursor) -> Token {
  while (std::isdigit(cursor.peek())) {
    cursor.advance();
  }

  // Look for a fractional part.
  if (cursor.peek() == '.' && std::isdigit(cursor.peek(1))) {
    // Consume the "."
    cursor.advance();

    while (std::isdigit(cursor.peek())) {
      cursor.advance();
    }
  }

  return make_token(cursor, TokenType::NUMBER, std::stod(cursor.peek_word()));
}

[[nodiscard]] auto handle_identifier(Cursor& cursor) -> Token {
  while (is_word_char(cursor.peek())) {
    cursor.advance();
  }

  std::string text = cursor.peek_word();
  // Text is either a reserved keyword, or a regular user-defined identifier
  auto const token_type =
      match_keyword_token_type(text).value_or(TokenType::IDENTIFIER);
  return make_token(cursor, token_type);
}

[[nodiscard]] auto build_special_character_token(Cursor& cursor, char const c)
    -> std::optional<Token> {
  // For chars that definitely make a single char token
  auto const single_char = [&cursor](auto const token_type) {
    return make_token(cursor, token_type);
  };

  // For characters that may make a double char token when followed by '='
  auto const single_or_double_char = [&cursor](auto const with_equal,
                                               auto const without_equal) {
    if (cursor.match('=')) {
      cursor.advance();
      return make_token(cursor, with_equal);
    }
    return make_token(cursor, without_equal);
  };

  switch (c) {
    case '(':
      return single_char(TokenType::LEFT_PAREN);
    case ')':
      return single_char(TokenType::RIGHT_PAREN);
    case '{':
      return single_char(TokenType::LEFT_BRACE);
    case '}':
      return single_char(TokenType::RIGHT_BRACE);
    case ',':
      return single_char(TokenType::COMMA);
    case '.':
      return single_char(TokenType::DOT);
    case '-':
      return single_char(TokenType::MINUS);
    case '+':
      return single_char(TokenType::PLUS);
    case ';':
      return single_char(TokenType::SEMICOLON);
    case '*':
      return single_char(TokenType::STAR);
    case '!':
      return single_or_double_char(TokenType::BANG_EQUAL, TokenType::BANG);
    case '=':
      return single_or_double_char(TokenType::EQUAL_EQUAL, TokenType::EQUAL);
    case '<':
      return single_or_double_char(TokenType::LESS_EQUAL, TokenType::LESS);
    case '>':
      return single_or_double_char(TokenType::GREATER_EQUAL,
                                   TokenType::GREATER);
    default:
      return std::nullopt;
  }
}

[[nodiscard]] auto handle_slash(Cursor& cursor) -> std::optional<Token> {
  // Comments start with a double slash.
  if (cursor.match('/')) {
    // A comment goes until the end of the line.
    while (cursor.peek() != '\n' && !cursor.is_at_end()) {
      cursor.advance();
    }
    return std::nullopt;
  }

  // Not a comment
  return make_token(cursor, TokenType::SLASH);
}

auto handle_whitespace(Cursor& cursor) -> std::nullopt_t {
  cursor.advance_word();
  return std::nullopt;
}

auto handle_newline(Cursor& cursor) -> std::nullopt_t {
  cursor.down();
  return std::nullopt;
}

[[nodiscard]] auto scan_token(Cursor& cursor) -> std::optional<Token> {
  auto const c = cursor.take();

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

  auto const message = "Unexpected character '" + std::string(1, c) + "'";
  throw LoxError(cursor.at_line(), message);
}
}  // namespace

namespace Scanner {
[[nodiscard]] auto scan_tokens(std::string const& contents)
    -> std::vector<Token const> {
  std::vector<Token const> tokens;
  Cursor cursor(contents);

  while (!cursor.is_at_end()) {
    cursor.advance_word();
    if (auto const token = scan_token(cursor)) {
      tokens.push_back(token.value());
    }
  }

  tokens.emplace_back(
      Token{TokenType::EOFF, "", std::monostate{}, cursor.at_line()});

  return tokens;
}
}  // namespace Scanner
