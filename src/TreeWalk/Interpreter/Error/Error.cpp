#include "./Error.hpp"

#include <string>

namespace LOX::TreeWalk::Interpreter {

Error::Error(std::size_t line, std::string const& message)
    : Interface("[line " + std::to_string(line) +
                "] Interpreter error: " + message + "\n") {}

}  // namespace LOX::TreeWalk::Interpreter
