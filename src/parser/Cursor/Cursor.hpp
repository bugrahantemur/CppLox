#ifndef LOX_PARSER_CURSOR
#define LOX_PARSER_CURSOR

#include <cassert>
#include <initializer_list>
#include <string>
#include <vector>

#include "../../Types/Token/Token.hpp"

namespace LOX::Parser {

class Cursor {
  std::vector<Token> const& tokens_;
  std::size_t current_;

 public:
  explicit Cursor(std::vector<Token> const& tokens);

  [[nodiscard]] auto match(TokenType types) const -> bool;

  [[nodiscard]] auto match_any_of(std::initializer_list<TokenType> types) const
      -> bool;

  [[nodiscard]] auto peek() const -> Token;

  [[nodiscard]] auto is_at_end() const -> bool;

  auto take() -> Token;

  auto consume(TokenType const& type, std::string const& message) -> Token;

  auto previous() -> Token;

  auto synchronize() -> void;
};
}  // namespace LOX::Parser

#endif
