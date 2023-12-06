#include "token.h"

#include <string>
#include <variant>

#include "../magic_enum/include/magic_enum/magic_enum.hpp"

auto Token::to_string() const -> std::string {
  auto const type_name = std::string{magic_enum::enum_name(type_)};

  struct ToString {
    std::string operator()(std::string const& str) { return str; }

    std::string operator()(double number) { return std::to_string(number); }

    std::string operator()(std::monostate const& none) { return ""; }
  };

  return type_name + " " + lexeme_ + " " + std::visit(ToString{}, literal_) +
         " ";
}

auto match_keyword_token_type(std::string const& text)
    -> std::optional<TokenType> {
  if (text == "and") return TokenType::AND;
  if (text == "class") return TokenType::CLASS;
  if (text == "else") return TokenType::ELSE;
  if (text == "for") return TokenType::FOR;
  if (text == "fun") return TokenType::FUN;
  if (text == "if") return TokenType::IF;
  if (text == "nil") return TokenType::NIL;
  if (text == "or") return TokenType::OR;
  if (text == "print") return TokenType::PRINT;
  if (text == "return") return TokenType::RETURN;
  if (text == "super") return TokenType::SUPER;
  if (text == "this") return TokenType::THIS;
  if (text == "true") return TokenType::TRUE;
  if (text == "var") return TokenType::VAR;
  if (text == "while") return TokenType::WHILE;

  return std::nullopt;
}
