#include "./Error.hpp"

#include <string>

namespace LOX::Common::Resolver {

Error::Error(std::size_t line, std::string const& message)
    : Interface("[line " + std::to_string(line) +
                "] Resolver error: " + message + "\n") {}

}  // namespace LOX::Common::Resolver
