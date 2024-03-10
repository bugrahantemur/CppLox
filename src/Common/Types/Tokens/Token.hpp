#ifndef LOX_COMMON_TYPES_TOKEN
#define LOX_COMMON_TYPES_TOKEN

#include <string>
#include <variant>

#include "./TokenTypes.hpp"

namespace LOX::Common::Types {

struct Token {
  using Literal = std::variant<std::monostate, bool, double, std::string>;

  Tokens::TokenType type;
  std::string lexeme;
  Literal literal;
  std::size_t line;
  std::size_t token_id;

  auto operator==(Token const& other) const -> bool;

  static auto none() -> Token;
};

}  // namespace LOX::Common::Types

template <>
struct std::hash<LOX::Common::Types::Token> {
  auto operator()(LOX::Common::Types::Token const& token) const -> std::size_t;
};

#endif
