#ifndef LOX_PARSER
#define LOX_PARSER

#include <vector>

#include "./types/statement.hpp"
#include "./types/token.hpp"
#include "./utils/error.hpp"

namespace Parser {
struct Error : LoxError {
  Error(std::size_t line, std::string const& where, std::string const& message);

  std::size_t line_;
  std::string where_;
  std::string message_;

  auto report() const -> void final;

 private:
  std::vector<Token> tokens_;
};

auto parse(std::vector<Token const> const& tokens)
    -> std::vector<Statement const>;
}  // namespace Parser

#endif
