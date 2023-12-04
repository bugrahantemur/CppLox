#include "parser.h"

#include <vector>

#include "expression.h"
#include "token.h"

namespace {

class TokenCursor {
 public:
  explicit TokenCursor(std::vector<Token>&& tokens)
      : tokens_(std::move(tokens)), current_(0){};

  template <typename Type>
  [[nodiscard]] auto match(Type type) const -> bool {
    return !is_at_end() && peek().type() == type;
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
    return peek().type() == TokenType::EOFF;
  }

 private:
  std::vector<Token> tokens_;
  std::size_t current_;
};

// Forward declare expression
auto expression(TokenCursor&) -> Expression;

auto primary(TokenCursor& tc) -> Expression {
  if (tc.match(TokenType::FALSE)) {
    return LiteralExpression{false};
  }
  if (tc.match(TokenType::TRUE)) {
    return LiteralExpression{true};
  }
  if (tc.match(TokenType::NIL)) {
    return LiteralExpression{std::monostate{}};
  }
  if (tc.match(TokenType::NUMBER, TokenType::STRING)) {
    return LiteralExpression{tc.take().literal()};
  }
  if (tc.match(TokenType::LEFT_PAREN)) {
    auto const expr = expression(tc);
    // TODO await right parantheses, compare to consume()
    return GroupingExpression{expr};
  }

  return LiteralExpression{std::monostate{}};  // TODO check this
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

}  // namespace
