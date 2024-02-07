#include "./Token.hpp"

#include "./TokenTypes.hpp"

namespace LOX::Types::Tokens {

auto Token::operator==(Token const& other) const -> bool {
  return token_id == other.token_id;
}

auto Token::none() -> Token {
  return Token{TokenType::EOFF, "", std::monostate{}, 0, 0};
}

}  // namespace LOX::Types::Tokens

auto std::hash<LOX::Types::Tokens::Token>::operator()(
    LOX::Types::Tokens::Token const& token) const -> std::size_t {
  return token.token_id;
}
