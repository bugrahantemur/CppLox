#include "./Error.hpp"

#include <string>

namespace LOX::Resolver {

Error::Error(std::size_t line, std::string const& message)
    : ErrorInterface("[line " + std::to_string(line) +
                     "] Resolver error: " + message + "\n") {}

}  // namespace LOX::Resolver
