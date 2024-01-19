#ifndef LOX_SCANNER_ERROR
#define LOX_SCANNER_ERROR

#include <exception>
#include <string>

#include "../utils/error_interface.hpp"

namespace LOX::Lexer {

class Error : ErrorInterface {
 public:
  Error(std::size_t const line, std::string const& message);

  auto report() const -> void final;

 private:
  std::size_t line_;
  std::string message_;
};

auto error(std::size_t const line, std::string message) -> void;

}  // namespace LOX::Lexer

#endif
