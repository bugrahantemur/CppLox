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
  std::size_t token_id_;

  auto operator==(Token const& other) const -> bool {
    return token_id_ == other.token_id_;
  }

  static auto none() -> Token {
    return Token{TokenType::EOFF, "", std::monostate{}, 0, 0};
  }
};

template <>
struct std::hash<Token> {
  auto operator()(Token const& token) const -> std::size_t {
    return token.token_id_;
  }
};

#endif
