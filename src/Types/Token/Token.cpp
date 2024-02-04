#include "./Token.hpp"

namespace LOX {

auto Token::operator==(Token const& other) const -> bool {
  return token_id == other.token_id;
}

auto Token::none() -> Token {
  return Token{TokenType::EOFF, "", std::monostate{}, 0, 0};
}

}  // namespace LOX

auto std::hash<LOX::Token>::operator()(LOX::Token const& token) const
    -> std::size_t {
  return token.token_id;
}
