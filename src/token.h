#ifndef LOX_TOKEN
#define LOX_TOKEN

#include <string>
#include <variant>

#include "./token_types.h"

class Token {
  using Literal = std::variant<std::monostate, std::string, double>;

 public:
  Token(TokenType type, std::string lexeme, Literal literal, int line);

  auto to_string() const -> std::string;

 private:
  TokenType type_;
  std::string lexeme_;
  Literal literal_;
  int line_;
};

#endif
