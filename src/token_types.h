#ifndef LOX_TOKEN_TYPES
#define LOX_TOKEN_TYPES

enum class TokenType {
  LEFT_PAREN,
  RIGHT_PAREN,
  LEFT_BRACE,
  RIGHT_BRACE,
  COMMA,
  DOT,
  MINUS,
  PLUS,
  SEMICOLON,
  SLASH,
  STAR,

  BANG,
  BANG_EQUAL,
  EQUAL,
  EQUAL_EQUAL,
  GREATER,
  GREATER_EQUAL,
  LESS,
  LESS_EQUAL,

  IDENTIFIER,
  STRING,
  NUMBER,

  AND,
  CLASS,
  ELSE,
  FALSE,
  FUN,
  FOR,
  IF,
  NIL,
  OR,
  PRINT,
  RETURN,
  SUPER,
  THIS,
  TRUE,
  VAR,
  WHILE,

  EOFF
};

auto get_keyword_token_type(std::string const& identifier)
    -> std::optional<TokenType> {
  if (identifier == "and") return TokenType::AND;
  if (identifier == "class") return TokenType::CLASS;
  if (identifier == "else") return TokenType::ELSE;
  if (identifier == "for") return TokenType::FOR;
  if (identifier == "fun") return TokenType::FUN;
  if (identifier == "if") return TokenType::IF;
  if (identifier == "nil") return TokenType::NIL;
  if (identifier == "or") return TokenType::OR;
  if (identifier == "print") return TokenType::PRINT;
  if (identifier == "return") return TokenType::RETURN;
  if (identifier == "super") return TokenType::SUPER;
  if (identifier == "this") return TokenType::THIS;
  if (identifier == "true") return TokenType::TRUE;
  if (identifier == "var") return TokenType::VAR;
  if (identifier == "while") return TokenType::WHILE;

  return std::nullopt;
}

#endif
