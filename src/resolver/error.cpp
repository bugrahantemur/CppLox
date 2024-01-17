#include "./error.hpp"

#include <iostream>
#include <string>

namespace Resolver {
Error::Error(std::size_t line, std::string const& message)
    : line_(line), message_(message) {}

auto Error::report() const -> void {
  std::cerr << "[line " << line_ << "] Resolver error: " << message_ << '\n';
}
}  // namespace Resolver
