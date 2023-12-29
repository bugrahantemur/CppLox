#ifndef LOX_PARSER_ERROR
#define LOX_PARSER_ERROR

#include <iostream>
#include <string>
#include <vector>

#include "../types/token.hpp"
#include "../utils/error.hpp"

namespace Parser {
struct Error : LoxError {
  Error(std::size_t line, std::string const& where, std::string const& message)
      : line_(line), where_(where), message_(message), tokens_{} {}

  std::size_t line_;
  std::string where_;
  std::string message_;

  auto report() const -> void final {
    std::cerr << "[line " << line_ << "] Parsing error " << where_ << ": "
              << message_ << '\n';
  };

 private:
  std::vector<Token> tokens_;
};

inline auto error(Token const& token, std::string message) -> Error {
  return Error{token.line_,
               token.type_ == TokenType::EOFF ? "at the end"
                                              : " at '" + token.lexeme_ + "'",
               message};
}
}  // namespace Parser

#endif
