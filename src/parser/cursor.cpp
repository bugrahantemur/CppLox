#include "cursor.hpp"

#include <cassert>
#include <string>
#include <vector>

#include "../../magic_enum/include/magic_enum/magic_enum.hpp"
#include "../types/token.hpp"
#include "./error.hpp"

namespace LOX::Parser {

Cursor::Cursor(std::vector<Token> const& tokens)
    : tokens_(tokens), current_(0){};

template <typename Type>
auto Cursor::match(Type type) const -> bool {
  return !is_at_end() && peek().type_ == type;
}

template <typename Type, typename... Types>
auto Cursor::match(Type type, Types... types) const -> bool {
  return match(type) || match(types...);
}

auto Cursor::peek() const -> Token { return tokens_.at(current_); }

auto Cursor::is_at_end() const -> bool {
  return (current_ >= tokens_.size()) || (peek().type_ == TokenType::EOFF);
}

auto Cursor::take() -> Token { return tokens_.at(current_++); }

auto Cursor::take(TokenType const& type) -> Token {
  if (match(type)) {
    return take();
  }

  throw error(peek(), "Expected " + std::string{magic_enum::enum_name(type)});
}

auto Cursor::previous() -> Token {
  assert(current_ > 0);
  return tokens_.at(current_ - 1);
}

auto Cursor::synchronize() -> void {
  while (!is_at_end()) {
    if (match(TokenType::CLASS, TokenType::FUN, TokenType::VAR, TokenType::FOR,
              TokenType::IF, TokenType::WHILE, TokenType::PRINT,
              TokenType::RETURN)) {
      return;
    }

    take();

    if (match(TokenType::SEMICOLON)) {
      return;
    }
  }
}
}  // namespace LOX::Parser
