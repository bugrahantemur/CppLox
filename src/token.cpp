#include "token.h"

#include <string>
#include <variant>

#include "../ext/magic_enum/include/magic_enum/magic_enum.hpp"

struct ToString {
  std::string operator()(std::string const& str) { return str; }

  std::string operator()(double number) { return std::to_string(number); }

  std::string operator()(std::monostate const& none) { return ""; }
};

Token::Token(TokenType type, std::string lexeme, Literal literal, int line)
    : type_(type), lexeme_(lexeme), literal_(literal), line_(line) {}

auto Token::to_string() const -> std::string {
  auto const type_name = std::string{magic_enum::enum_name(type_)};

  return type_name + " " + lexeme_ + " " + std::visit(ToString{}, literal_);
}
