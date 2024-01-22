#include "./error.hpp"

#include <iostream>
#include <string>

namespace LOX::Lexer {

Error::Error(std::size_t const line, std::string const& message)
    : ErrorInterface("[line " + std::to_string(line) +
                     "] Scanning error: " + message + "\n") {}

auto error(std::size_t const line, std::string message) -> void {
  throw Error{line, message};
}

}  // namespace LOX::Lexer
