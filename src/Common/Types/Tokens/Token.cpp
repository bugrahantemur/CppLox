#include "./Token.hpp"

#include "./TokenTypes.hpp"

namespace LOX::Types {

auto Token::operator==(Token const& other) const -> bool {
  return token_id == other.token_id;
}

auto Token::none() -> Token {
  return Token{Tokens::TokenType::EOFF, "", std::monostate{}, 0, 0};
}

}  // namespace LOX::Types

auto std::hash<LOX::Types::Token>::operator()(
    LOX::Types::Token const& token) const -> std::size_t {
  return token.token_id;
}
