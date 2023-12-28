#ifndef LOX_SCANNER
#define LOX_SCANNER

#include <string>
#include <vector>

#include "./types/token.hpp"
#include "./utils/error.hpp"

namespace Scanner {
struct Error : LoxError {
  Error(std::size_t line, std::string const& message);

  std::size_t line_;
  std::string message_;

  auto report() const -> void final;
};

[[nodiscard]] auto scan_tokens(std::string const& contents)
    -> std::vector<Token>;
}  // namespace Scanner

#endif
