#include "Cursor.hpp"

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <string>
#include <vector>

#include "../../../Common/Types/Tokens/Token.hpp"
#include "../../../Common/Types/Tokens/TokenTypes.hpp"
#include "../Error/Error.hpp"

namespace LOX::TreeWalk::Parser {

using namespace LOX::Common::Types::Tokens;
using LOX::Common::Types::Token;

Cursor::Cursor(std::vector<Token> const& tokens) : tokens(tokens), current(0){};

auto Cursor::match(TokenType type) const -> bool {
  return !is_at_end() and peek().type == type;
}

auto Cursor::match_any_of(std::initializer_list<TokenType> types) const
    -> bool {
  return std::any_of(begin(types), end(types),
                     [this](TokenType const& type) { return match(type); });
}

auto Cursor::peek() const -> Token { return tokens.at(current); }

auto Cursor::is_at_end() const -> bool {
  return (current >= tokens.size()) || (peek().type == TokenType::EOFF);
}

auto Cursor::take() -> Token { return tokens.at(current++); }

auto Cursor::consume(TokenType const& type, std::string const& message)
    -> Token {
  if (match(type)) {
    return take();
  }

  throw error(peek(), message);
}

auto Cursor::previous() -> Token {
  assert(current > 0);
  return tokens.at(current - 1);
}

auto Cursor::synchronize() -> void {
  while (!is_at_end()) {
    if (match_any_of({TokenType::CLASS, TokenType::FUN, TokenType::VAR,
                      TokenType::FOR, TokenType::IF, TokenType::WHILE,
                      TokenType::PRINT, TokenType::RETURN})) {
      return;
    }

    take();

    if (match(TokenType::SEMICOLON)) {
      return;
    }
  }
}
}  // namespace LOX::TreeWalk::Parser
