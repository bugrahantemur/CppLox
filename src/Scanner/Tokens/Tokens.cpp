#include "./Tokens.hpp"

#include "../../Types/Tokens/Token.hpp"
#include "../../Types/Tokens/TokenTypes.hpp"
#include "../Cursor/Cursor.hpp"
#include "../Error/Error.hpp"

namespace LOX::Scanner {

using namespace Types::Tokens;

auto match_keyword_token_type(std::string const& text)
    -> std::optional<TokenType> {
  if (text == "and") return TokenType::AND;
  if (text == "class") return TokenType::CLASS;
  if (text == "else") return TokenType::ELSE;
  if (text == "false") return TokenType::FALSE;
  if (text == "for") return TokenType::FOR;
  if (text == "fun") return TokenType::FUN;
  if (text == "if") return TokenType::IF;
  if (text == "nil") return TokenType::NIL;
  if (text == "or") return TokenType::OR;
  if (text == "print") return TokenType::PRINT;
  if (text == "return") return TokenType::RETURN;
  if (text == "super") return TokenType::SUPER;
  if (text == "this") return TokenType::THIS;
  if (text == "true") return TokenType::TRUE;
  if (text == "var") return TokenType::VAR;
  if (text == "while") return TokenType::WHILE;

  return std::nullopt;
}

[[nodiscard]] auto is_word_char(char const c) -> bool {
  return std::isalnum(c) || c == '_';
}

[[nodiscard]] auto make_token(Cursor& cursor, TokenType token_type,
                              Token::Literal const& literal = std::monostate{})
    -> Token {
  std::string const text{cursor.peek_word()};
  return Token{token_type, text, literal, cursor.at_line(), cursor.index()};
}

[[nodiscard]] auto handle_string_literal(Cursor& cursor) -> Token {
  while (cursor.peek() != '"' and !cursor.is_at_end()) {
    if (cursor.peek() == '\n') {
      cursor.down();
    }
    cursor.advance();
  }

  if (cursor.is_at_end()) {
    error(cursor.at_line(), "Unterminated string literal");
  }

  // Closing double quotes
  cursor.advance();

  std::string const text{cursor.peek_word()};

  // Trim the surrounding quotes
  std::string const value{text.substr(1, text.size() - 2)};

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

  std::string text{cursor.peek_word()};
  // Text is either a reserved keyword, or a regular user-defined identifier
  TokenType const token_type{
      match_keyword_token_type(text).value_or(TokenType::IDENTIFIER)};
  return make_token(cursor, token_type);
}

[[nodiscard]] auto build_special_character_token(Cursor& cursor, char const c)
    -> std::optional<Token> {
  // For chars that definitely make a single char token
  auto const single_char{[&cursor](TokenType const token_type) {
    return make_token(cursor, token_type);
  }};

  // For characters that may make a double char token when followed by '='
  auto const single_or_double_char = [&cursor](TokenType const with_equal,
                                               TokenType const without_equal) {
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

}  // namespace LOX::Scanner
