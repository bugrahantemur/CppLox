#include "./Error.hpp"

#include <string>

#include "../../../Common/Types/Tokens/Token.hpp"
#include "../../../Common/Types/Tokens/TokenTypes.hpp"

namespace LOX::Common::Parser {

Error::Error(std::size_t const line, std::string const& where,
             std::string const& message)
    : Interface("[line " + std::to_string(line) + "] Parsing error " + where +
                ": " + message + "\n") {}

auto error(LOX::Common::Types::Token const& token, std::string const& message)
    -> Error {
  return Error{token.line,
               token.type == LOX::Common::Types::Tokens::TokenType::EOFF
                   ? "at the end"
                   : " at '" + token.lexeme + "'",
               message};
}

}  // namespace LOX::Common::Parser
