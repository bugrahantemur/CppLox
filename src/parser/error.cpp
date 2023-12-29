
#include "./error.hpp"

#include <iostream>
#include <string>
#include <vector>

#include "../types/token.hpp"
#include "../utils/error.hpp"

namespace Parser {
Error::Error(std::size_t line, std::string const& where,
             std::string const& message)
    : line_(line), where_(where), message_(message), tokens_{} {}

auto Error::report() const -> void {
  std::cerr << "[line " << line_ << "] Parsing error " << where_ << ": "
            << message_ << '\n';
}

auto error(Token const& token, std::string message) -> Error {
  return Error{token.line_,
               token.type_ == TokenType::EOFF ? "at the end"
                                              : " at '" + token.lexeme_ + "'",
               message};
}
}  // namespace Parser
