#ifndef LOX_PARSER_CURSOR
#define LOX_PARSER_CURSOR

#include <cassert>
#include <string>
#include <vector>

#include "../token/token.hpp"
#include "./error.hpp"

namespace LOX::Parser {

class Cursor {
  std::vector<Token> const& tokens_;
  std::size_t current_;

 public:
  explicit Cursor(std::vector<Token> const& tokens);

  template <typename Type>
  [[nodiscard]] auto match(Type type) const -> bool;

  template <typename Type, typename... Types>
  [[nodiscard]] auto match(Type type, Types... types) const -> bool;

  [[nodiscard]] auto peek() const -> Token;

  [[nodiscard]] auto is_at_end() const -> bool;

  auto take() -> Token;

  auto consume(TokenType const& type, std::string const& message) -> Token;

  auto previous() -> Token;

  auto synchronize() -> void;
};
}  // namespace LOX::Parser

#endif
