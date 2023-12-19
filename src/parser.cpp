#include "parser.h"

#include <iostream>
#include <vector>

#include "../magic_enum/include/magic_enum/magic_enum.hpp"
#include "expression.h"
#include "statement.h"
#include "token.h"
#include "utils/error.h"

namespace {

class TokenCursor {
 public:
  explicit TokenCursor(std::vector<Token const> const& tokens)
      : tokens_(tokens), current_(0){};

  template <typename Type>
  [[nodiscard]] auto match(Type type) const -> bool {
    return !is_at_end() && peek().type_ == type;
  }
  template <typename Type, typename... Types>
  [[nodiscard]] auto match(Type type, Types... types) const -> bool {
    return match(type) || match(types...);
  }

  [[nodiscard]] auto take() -> Token {
    auto const tmp = peek();
    advance();
    return tmp;
  }

  auto advance() -> void { current_++; }

  [[nodiscard]] auto peek() const -> Token { return tokens_.at(current_); }

  [[nodiscard]] auto is_at_end() const -> bool {
    return peek().type_ == TokenType::EOFF;
  }

 private:
  auto synchronize() -> void {
    advance();

    while (!is_at_end()) {
      if (match(TokenType::CLASS, TokenType::FUN, TokenType::VAR,
                TokenType::FOR, TokenType::IF, TokenType::WHILE,
                TokenType::PRINT, TokenType::RETURN)) {
        return;
      }

      advance();

      if (match(TokenType::SEMICOLON)) {
        return;
      }
    }
  }

  std::vector<Token const> const& tokens_;
  std::size_t current_;
};

class ParserError : public std::exception {
 public:
  ParserError() = default;

  auto what() const noexcept -> char const* final { return "Parser error"; }

 private:
  std::vector<Token> tokens_;
};

auto error(Token const& token, std::string message) -> void {
  if (token.type_ == TokenType::EOFF) {
    report(token.line_, "at the end", message);
  } else {
    report(token.line_, " at '" + token.lexeme_ + "'", message);
  }
}

auto expect(TokenCursor& tc, TokenType const& type) -> void {
  if (tc.match(type)) {
    tc.advance();
    return;
  }

  error(tc.peek(), "Expected " + std::string{magic_enum::enum_name(type)});
  throw ParserError{};
}

// Forward declare expression()
auto expression(TokenCursor&) -> Expression;

auto primary(TokenCursor& tc) -> Expression {
  if (tc.match(TokenType::FALSE)) {
    tc.advance();
    return LiteralExpression{false};
  }
  if (tc.match(TokenType::TRUE)) {
    tc.advance();
    return LiteralExpression{true};
  }
  if (tc.match(TokenType::NIL)) {
    tc.advance();
    return LiteralExpression{std::monostate{}};
  }
  if (tc.match(TokenType::NUMBER, TokenType::STRING)) {
    return LiteralExpression{tc.take().literal_};
  }
  if (tc.match(TokenType::LEFT_PAREN)) {
    tc.advance();
    auto const expr = expression(tc);
    expect(tc, TokenType::RIGHT_PAREN);
    // expect() may throw, in this case unrecoverably
    return GroupingExpression{expr};
  }

  error(tc.peek(), "Expect expression.");
  throw ParserError{};
}

auto unary(TokenCursor& tc) -> Expression {
  if (tc.match(TokenType::BANG, TokenType::MINUS)) {
    auto const op = tc.take();
    auto const right = unary(tc);
    return UnaryExpression{op, right};
  }

  return primary(tc);
}

template <typename F, typename... Types>
auto sequence(TokenCursor& tc, F const& f, Types... types) -> Expression {
  auto expr = f(tc);

  while (tc.match(types...)) {
    auto const op = tc.take();
    auto const right = f(tc);
    expr = BinaryExpression{expr, op, right};
  }

  return expr;
}

auto factor(TokenCursor& tc) -> Expression {
  return sequence(tc, unary, TokenType::SLASH, TokenType::STAR);
};

auto term(TokenCursor& tc) -> Expression {
  return sequence(tc, factor, TokenType::MINUS, TokenType::PLUS);
}

auto comparison(TokenCursor& tc) -> Expression {
  return sequence(tc, term, TokenType::GREATER_EQUAL, TokenType::GREATER,
                  TokenType::LESS_EQUAL, TokenType::LESS);
};

auto equality(TokenCursor& tc) -> Expression {
  return sequence(tc, comparison, TokenType::BANG_EQUAL,
                  TokenType::EQUAL_EQUAL);
};

auto expression(TokenCursor& tc) -> Expression { return equality(tc); };

auto print_statement(TokenCursor& tc) -> Statement {
  Expression const value = expression(tc);
  expect(tc, TokenType::SEMICOLON);
  return PrintStatement{value};
}

auto expression_statement(TokenCursor& tc) -> Statement {
  Expression const expr = expression(tc);
  expect(tc, TokenType::SEMICOLON);
  return ExpressionStatement{expr};
}

auto statement(TokenCursor& tc) -> Statement {
  if (tc.match(TokenType::PRINT)) {
    tc.advance();
    return print_statement(tc);
  }

  return expression_statement(tc);
}
}  // namespace

namespace Parser {
auto parse(std::vector<Token const> const& tokens)
    -> std::vector<Statement const> {
  TokenCursor tc(tokens);

  std::vector<Statement const> statements{};

  while (!tc.is_at_end()) {
    statements.push_back(statement(tc));
  }

  return statements;
}
}  // namespace Parser
