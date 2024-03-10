#ifndef LOX_COMMON_SCANNER_CURSOR
#define LOX_COMMON_SCANNER_CURSOR

#include <cassert>
#include <string>

namespace LOX::Common::Scanner {

class Cursor {
 public:
  Cursor(std::string const& source);

  auto advance() -> void;

  auto down() -> void;

  auto advance_word() -> void;

  [[nodiscard]] auto peek(std::size_t forward = 0) const -> char;

  [[nodiscard]] auto take() -> char;

  [[nodiscard]] auto peek_word() const -> std::string;

  [[nodiscard]] auto at_line() const -> std::size_t;

  [[nodiscard]] auto is_at_end() const -> bool;

  [[nodiscard]] auto match(char const expected) const -> bool;

  [[nodiscard]] auto index() const -> std::size_t;

 private:
  std::string const& source;
  std::size_t start;
  std::size_t current;
  std::size_t line;
};

}  // namespace LOX::Common::Scanner
#endif
