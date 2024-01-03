#ifndef LOX_PARSER_ERROR
#define LOX_PARSER_ERROR

#include <string>
#include <vector>

#include "../types/token.hpp"
#include "../utils/error.hpp"

namespace Parser {
struct Error : CompileTimeError {
  Error(std::size_t line, std::string const& where, std::string const& message);

  std::size_t line_;
  std::string where_;
  std::string message_;

  auto report() const -> void final;

 private:
  std::vector<Token> tokens_;
};

auto error(Token const& token, std::string message) -> Error;
}  // namespace Parser

#endif
