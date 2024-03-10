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
  std::vector<LOX::Common::Types::Token> const& tokens;
  std::size_t current;

 public:
  explicit Cursor(std::vector<LOX::Common::Types::Token> const& tokens);

  [[nodiscard]] auto match(LOX::Common::Types::Tokens::TokenType types) const
      -> bool;

  [[nodiscard]] auto match_any_of(
      std::initializer_list<LOX::Common::Types::Tokens::TokenType> types) const
      -> bool;

  [[nodiscard]] auto peek() const -> LOX::Common::Types::Token;

  [[nodiscard]] auto is_at_end() const -> bool;

  auto take() -> LOX::Common::Types::Token;

  auto consume(LOX::Common::Types::Tokens::TokenType const& type,
               std::string const& message) -> LOX::Common::Types::Token;

  auto previous() -> LOX::Common::Types::Token;

  auto synchronize() -> void;
};
}  // namespace LOX::TreeWalk::Parser

#endif
