#include "./Error.hpp"

#include <string>

namespace LOX::Reader {

Error::Error(std::string const &message)
    : Interface("File reader error: " + message) {}

}  // namespace LOX::Reader
