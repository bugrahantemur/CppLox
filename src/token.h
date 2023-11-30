#ifndef LOX_TOKEN
#define LOX_TOKEN

#include <optional>
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

class Token {
  using Literal = std::variant<std::monostate, std::string, double>;

 public:
  Token(TokenType type, std::string lexeme, Literal literal, std::size_t line);

  auto to_string() const -> std::string;

 private:
  TokenType type_;
  std::string lexeme_;
  Literal literal_;
  std::size_t line_;
};

auto match_keyword_token_type(std::string const& text)
    -> std::optional<TokenType>;

#endif
