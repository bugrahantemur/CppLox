#ifndef LOX_PARSER_CURSOR
#define LOX_PARSER_CURSOR

#include <cassert>
#include <initializer_list>
#include <string>
#include <vector>

#include "../../../Common/Types/Tokens/Token.hpp"
#include "../../../Common/Types/Tokens/TokenTypes.hpp"

namespace LOX::TreeWalk::Parser {

class Cursor {
  std::vector<LOX::Types::Token> const& tokens;
  std::size_t current;

 public:
  explicit Cursor(std::vector<LOX::Types::Token> const& tokens);

  [[nodiscard]] auto match(LOX::Types::Tokens::TokenType types) const -> bool;

  [[nodiscard]] auto match_any_of(
      std::initializer_list<LOX::Types::Tokens::TokenType> types) const -> bool;

  [[nodiscard]] auto peek() const -> LOX::Types::Token;

  [[nodiscard]] auto is_at_end() const -> bool;

  auto take() -> LOX::Types::Token;

  auto consume(LOX::Types::Tokens::TokenType const& type,
               std::string const& message) -> LOX::Types::Token;

  auto previous() -> LOX::Types::Token;

  auto synchronize() -> void;
};
}  // namespace LOX::TreeWalk::Parser

#endif
