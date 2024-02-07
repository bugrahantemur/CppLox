#include "./Error.hpp"

#include <string>

namespace LOX::Interpreter {

Error::Error(std::size_t line, std::string const& message)
    : ErrorInterface("[line " + std::to_string(line) +
                     "] Interpreter error: " + message + "\n") {}

}  // namespace LOX::Interpreter
