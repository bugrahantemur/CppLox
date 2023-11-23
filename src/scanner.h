
#ifndef LOX_SCANNER
#define LOX_SCANNER

#include <optional>
#include <string>
#include <vector>

#include "token.h"
#include "token_types.h"

class Scanner {
  using Literal = std::variant<std::monostate, std::string, double>;

 public:
  Scanner(std::string const& source);

  auto scan_tokens() -> std::vector<Token>;

 private:
  [[nodiscard]] auto is_at_end() const -> bool;

  [[nodiscard]] auto peek() const -> char;

  [[nodiscard]] auto peek_next() const -> char;

  [[nodiscard]] auto match(char expected) -> bool;

  auto scan_token() -> void;

  auto advance() -> char;

  auto add_token(TokenType token_type) -> void;

  auto add_token(TokenType token_type, Literal const& literal) -> void;

  auto handle_string_literal() -> void;

  auto handle_number_literal() -> void;

  auto handle_identifier() -> void;

  std::string source_;
  std::vector<Token> tokens_;

  int start_{0};
  int current_{0};
  int line_{0};
};

#endif
