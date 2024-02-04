#ifndef LOX_TOKEN
#define LOX_TOKEN

#include <string>
#include <variant>

namespace LOX {

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

  TokenType type;
  std::string lexeme;
  Literal literal;
  std::size_t line;
  std::size_t token_id;

  auto operator==(Token const& other) const -> bool;

  static auto none() -> Token;
};

}  // namespace LOX

template <>
struct std::hash<LOX::Token> {
  auto operator()(LOX::Token const& token) const -> std::size_t;
};

#endif
