#ifndef LOX_PARSER_ERROR
#define LOX_PARSER_ERROR

#include <exception>
#include <string>
#include <vector>

#include "../types/token.hpp"

namespace Parser {
class Error : std::exception {
 public:
  Error(std::size_t const line, std::string const& where,
        std::string const& message);

  auto report() const -> void;

 private:
  std::size_t line_;
  std::string where_;
  std::string message_;
};

auto error(Token const& token, std::string const& message) -> Error;
}  // namespace Parser

#endif
