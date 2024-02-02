#include "Cursor.hpp"

#include <algorithm>
#include <cassert>
#include <initializer_list>
#include <string>
#include <vector>

#include "../../Types/Token/Token.hpp"
#include "../Error/Error.hpp"

namespace LOX::Parser {

Cursor::Cursor(std::vector<Token> const& tokens)
    : tokens_(tokens), current_(0){};

auto Cursor::match(TokenType type) const -> bool {
  return !is_at_end() and peek().type_ == type;
}

auto Cursor::match_any_of(std::initializer_list<TokenType> types) const
    -> bool {
  return std::any_of(begin(types), end(types),
                     [this](TokenType const& type) { return match(type); });
}

auto Cursor::peek() const -> Token { return tokens_.at(current_); }

auto Cursor::is_at_end() const -> bool {
  return (current_ >= tokens_.size()) || (peek().type_ == TokenType::EOFF);
}

auto Cursor::take() -> Token { return tokens_.at(current_++); }

auto Cursor::consume(TokenType const& type, std::string const& message)
    -> Token {
  if (match(type)) {
    return take();
  }

  throw error(peek(), message);
}

auto Cursor::previous() -> Token {
  assert(current_ > 0);
  return tokens_.at(current_ - 1);
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
}  // namespace LOX::Parser
