#ifndef LOX_TYPES_TOKEN
#define LOX_TYPES_TOKEN

#include <string>
#include <variant>

#include "./TokenTypes.hpp"

namespace LOX::Types::Tokens {

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

}  // namespace LOX::Types::Tokens

template <>
struct std::hash<LOX::Types::Tokens::Token> {
  auto operator()(LOX::Types::Tokens::Token const& token) const -> std::size_t;
};

#endif
