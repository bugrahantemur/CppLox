#include "./error.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "../syntax/token.hpp"

namespace LOX::Parser {

Error::Error(std::size_t const line, std::string const& where,
             std::string const& message)
    : ErrorInterface("[line " + std::to_string(line) + "] Parsing error " +
                     where + ": " + message + "\n") {}

auto error(Token const& token, std::string const& message) -> Error {
  return Error{token.line_,
               token.type_ == TokenType::EOFF ? "at the end"
                                              : " at '" + token.lexeme_ + "'",
               message};
}

}  // namespace LOX::Parser
