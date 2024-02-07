#ifndef LOX_PARSER_CURSOR
#define LOX_PARSER_CURSOR

#include <cassert>
#include <initializer_list>
#include <string>
#include <vector>

#include "../../Types/Tokens/Token.hpp"
#include "../../Types/Tokens/TokenTypes.hpp"

namespace LOX::Parser {

class Cursor {
  std::vector<Types::Tokens::Token> const& tokens;
  std::size_t current;

 public:
  explicit Cursor(std::vector<Types::Tokens::Token> const& tokens);

  [[nodiscard]] auto match(Types::Tokens::TokenType types) const -> bool;

  [[nodiscard]] auto match_any_of(
      std::initializer_list<Types::Tokens::TokenType> types) const -> bool;

  [[nodiscard]] auto peek() const -> Types::Tokens::Token;

  [[nodiscard]] auto is_at_end() const -> bool;

  auto take() -> Types::Tokens::Token;

  auto consume(Types::Tokens::TokenType const& type, std::string const& message)
      -> Types::Tokens::Token;

  auto previous() -> Types::Tokens::Token;

  auto synchronize() -> void;
};
}  // namespace LOX::Parser

#endif
