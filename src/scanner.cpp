#include "scanner.h"

#include <optional>
#include <string>
#include <vector>

#include "token.h"
#include "utils/error.h"

/**
 * Check if a character is a word character.
 *
 * @param c the character to check
 *
 * @return true if the character is a word character, false otherwise
 */
[[nodiscard]] auto is_word_char(char c) -> bool {
  return std::isalnum(c) || c == '_';
}

Scanner::Scanner(std::string const& source) : source_(source) {}

auto Scanner::scan_tokens() -> std::vector<Token> {
  while (not is_at_end()) {
    start_ = current_;
    scan_token();
  }

  tokens_.emplace_back(TokenType::EOFF, "", std::monostate{}, line_);

  return tokens_;
  ;
}

auto Scanner::is_at_end() const -> bool { return current_ >= source_.size(); }

auto Scanner::peek() const -> char {
  if (is_at_end()) return '\0';

  return source_.at(current_);
}

auto Scanner::peek_next() const -> char {
  if (current_ + 1 >= source_.length()) return '\0';

  return source_.at(current_ + 1);
}

auto Scanner::match(char expected) -> bool {
  if (is_at_end()) return false;

  if (source_.at(current_) != expected) return false;

  // Consume the character if matched
  current_++;

  return true;
}

auto Scanner::advance() -> char { return source_.at(current_++); }

auto Scanner::add_token(TokenType token_type) -> void {
  add_token(token_type, std::monostate{});
}

auto Scanner::add_token(TokenType token_type, Literal const& literal) -> void {
  std::string text = source_.substr(start_, current_ - start_);
  tokens_.emplace_back(token_type, text, literal, line_);
}

auto Scanner::handle_string_literal() -> void {
  while (peek() != '"' and not is_at_end()) {
    if (peek() == '\n') line_++;
    advance();
  }

  if (is_at_end()) {
    // Error
    return;
  }

  advance();  // Closing "

  std::string value = source_.substr(start_ + 1, current_ - start_ - 2);
  add_token(TokenType::STRING, value);
}

auto Scanner::handle_number_literal() -> void {
  while (std::isdigit(peek())) {
    advance();
  }

  // Look for a fractional part.
  if (peek() == '.' && std::isdigit(peek_next())) {
    // Consume the "."
    advance();

    while (std::isdigit(peek())) {
      advance();
    }
  }

  add_token(TokenType::NUMBER, std::stod(source_.substr(start_, current_)));
}

auto Scanner::handle_identifier() -> void {
  while (is_word_char(peek())) {
    advance();
  }

  std::string const& text = source_.substr(start_, current_);
  // Text is either a reserved keyword, or a regular user-defined identifier
  auto const token_type =
      match_keyword_token_type(text).value_or(TokenType::IDENTIFIER);
  add_token(token_type);
}

auto Scanner::scan_token() -> void {
  char c = advance();
  switch (c) {
    // Single character tokens
    case '(':
      add_token(TokenType::LEFT_PAREN);
      break;
    case ')':
      add_token(TokenType::RIGHT_PAREN);
      break;
    case '{':
      add_token(TokenType::LEFT_BRACE);
      break;
    case '}':
      add_token(TokenType::RIGHT_BRACE);
      break;
    case ',':
      add_token(TokenType::COMMA);
      break;
    case '.':
      add_token(TokenType::DOT);
      break;
    case '-':
      add_token(TokenType::MINUS);
      break;
    case '+':
      add_token(TokenType::PLUS);
      break;
    case ';':
      add_token(TokenType::SEMICOLON);
      break;
    case '*':
      add_token(TokenType::STAR);
      break;
    // Possibly two character tokens
    case '!':
      add_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
      break;
    case '=':
      add_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
      break;
    case '<':
      add_token(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
      break;
    case '>':
      add_token(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
    case '/':
      // Comments start with a double slash.
      if (match('/')) {
        // A comment goes until the end of the line.
        while (peek() != '\n' && !is_at_end()) {
          advance();
        }
      } else {  // Not a comment
        add_token(TokenType::SLASH);
      }
      break;
    // Whitespace
    case ' ':
    case '\r':
    case '\t':
      // Ignore whitespace
      break;
    // New line
    case '\n':
      line_++;
      break;
    // String literals start with double quotes
    case '"':
      handle_string_literal();
      break;
    default:
      if (std::isdigit(c)) {
        handle_number_literal();
      } else if (is_word_char(c)) {
        handle_identifier();
      } else {
        auto const message = "Unexpected character '" + std::string(1, c) + "'";
        throw LoxError(line_, message);
      }
      break;
  }
}
