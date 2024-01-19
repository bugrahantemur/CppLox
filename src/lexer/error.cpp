#include "./error.hpp"

#include <iostream>
#include <string>

namespace LOX::Lexer {

Error::Error(std::size_t const line, std::string const& message)
    : line_(line), message_(message) {}

auto Error::report() const -> void {
  std::cerr << "[line " << line_ << "] Scanning error: "
            << ": " << message_ << '\n';
}

auto error(std::size_t const line, std::string message) -> void {
  throw Error{line, message};
}

}  // namespace LOX::Lexer
