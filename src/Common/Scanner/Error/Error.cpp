#include "./Error.hpp"

#include <string>

namespace LOX::Scanner {

Error::Error(std::size_t const line, std::string const& message)
    : Interface("[line " + std::to_string(line) +
                "] Scanning error: " + message + "\n") {}

auto error(std::size_t const line, std::string message) -> void {
  throw Error{line, message};
}

}  // namespace LOX::Scanner
