#include "./Error.hpp"

#include <string>

#include "../../Types/Tokens/Token.hpp"
#include "../../Types/Tokens/TokenTypes.hpp"

namespace LOX::Parser {

Error::Error(std::size_t const line, std::string const& where,
             std::string const& message)
    : ErrorInterface("[line " + std::to_string(line) + "] Parsing error " +
                     where + ": " + message + "\n") {}

auto error(Types::Tokens::Token const& token, std::string const& message)
    -> Error {
  return Error{token.line,
               token.type == Types::Tokens::TokenType::EOFF
                   ? "at the end"
                   : " at '" + token.lexeme + "'",
               message};
}

}  // namespace LOX::Parser
