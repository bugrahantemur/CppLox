#include "./Token.hpp"

#include "./TokenTypes.hpp"

namespace LOX::Common::Types {

auto Token::operator==(Token const& other) const -> bool {
  return token_id == other.token_id;
}

auto Token::none() -> Token {
  return Token{Tokens::TokenType::EOFF, "", std::monostate{}, 0, 0};
}

}  // namespace LOX::Common::Types

auto std::hash<LOX::Common::Types::Token>::operator()(
    LOX::Common::Types::Token const& token) const -> std::size_t {
  return token.token_id;
}
