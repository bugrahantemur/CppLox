#include "./error.hpp"

#include <iostream>
#include <string>

namespace LOX::Interpreter {

Error::Error(std::size_t line, std::string const& message)
    : line_(line), message_(message) {}

auto Error::report() const -> void {
  std::cerr << "[line " << line_ << "] " << message_ << '\n';
}

}  // namespace LOX::Interpreter
