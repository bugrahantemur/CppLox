#ifndef LOX_SCANNER_ERROR
#define LOX_SCANNER_ERROR

#include <exception>
#include <string>

namespace LOX::Lexer {
class Error : std::exception {
 public:
  Error(std::size_t const line, std::string const& message);

  auto report() const -> void;

 private:
  std::size_t line_;
  std::string message_;
};

auto error(std::size_t const line, std::string message) -> void;

}  // namespace LOX::Lexer

#endif
