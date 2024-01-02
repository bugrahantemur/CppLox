#ifndef LOX_TYPES_TOKEN
#define LOX_TYPES_TOKEN

#include <string>
#include <variant>

enum class TokenType {
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  IDENTIFIER,
  STRING,
  NUMBER,

  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  EOFF
};

struct Token {
  using Literal = std::variant<std::monostate, bool, double, std::string>;

  TokenType type_;
  std::string lexeme_;
  Literal literal_;
  std::size_t line_;

  auto operator<(Token const& other) const -> bool {
    if (type_ != other.type_) {
      return type_ < other.type_;
    }
    if (lexeme_ != other.lexeme_) {
      return lexeme_ < other.lexeme_;
    }
    if (literal_ != other.literal_) {
      return literal_ < other.literal_;
    }
    return line_ < other.line_;
  }
};

#endif
