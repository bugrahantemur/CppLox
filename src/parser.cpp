#include "parser.h"

#include <iostream>
#include <vector>

#include "../magic_enum/include/magic_enum/magic_enum.hpp"
#include "./types/expression.h"
#include "./types/statement.h"
#include "./types/token.h"
#include "./utils/error.h"
namespace Parser {
Error::Error(std::size_t const line, std::string const& where,
             std::string const& message)
    : line_(line), where_(where), message_(message), tokens_{} {}

auto Error::report() const -> void {
  std::cerr << "[line " << line_ << "] Parsing error " << where_ << ": "
            << message_ << '\n';
}
}  // namespace Parser

namespace {
auto error(Token const& token, std::string message) -> Parser::Error {
  return Parser::Error{token.line_,
                       token.type_ == TokenType::EOFF
                           ? "at the end"
                           : " at '" + token.lexeme_ + "'",
                       message};
}

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
    Token const tmp{peek()};
    advance();
    return tmp;
  }

  auto advance() -> void { current_++; }

  [[nodiscard]] auto peek() const -> Token { return tokens_.at(current_); }

  [[nodiscard]] auto is_at_end() const -> bool {
    return (current_ >= tokens_.size()) || (peek().type_ == TokenType::EOFF);
  }

  auto consume(TokenType const& type) -> Token {
    if (match(type)) {
      return take();
    }

    throw error(peek(), "Expected " + std::string{magic_enum::enum_name(type)});
  }

  auto synchronize() -> void {
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

 private:
  std::vector<Token const> const& tokens_;
  std::size_t current_;
};

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
    Expression const expr{expression(tc)};
    tc.consume(TokenType::RIGHT_PAREN);
    return GroupingExpression{expr};
  }
  if (tc.match(TokenType::IDENTIFIER)) {
    return VariableExpression{tc.take()};
  }

  throw error(tc.peek(), "Expected expression.");
}

auto parse_args(TokenCursor& tc) -> std::vector<Expression> {
  std::vector<Expression> args{};

  if (!tc.match(TokenType::RIGHT_PAREN)) {
    args.push_back(expression(tc));
    while (tc.match(TokenType::COMMA)) {
      tc.advance();
      args.push_back(expression(tc));
    }
  }

  if (args.size() >= 255) {
    error(tc.peek(), "Can't have more than 255 arguments.").report();
  }

  return args;
}

auto call(TokenCursor& tc) -> Expression {
  Expression expr{primary(tc)};

  while (true) {
    if (tc.match(TokenType::LEFT_PAREN)) {
      tc.advance();
      std::vector<Expression> const arg{parse_args(tc)};
      return CallExpression{expr, tc.consume(TokenType::RIGHT_PAREN), arg};
    } else {
      break;
    }
  }

  return expr;
}

auto unary(TokenCursor& tc) -> Expression {
  if (tc.match(TokenType::BANG, TokenType::MINUS)) {
    Token const op{tc.take()};
    return UnaryExpression{op, unary(tc)};
  }

  return call(tc);
}

template <typename F, typename Result, typename... Types>
auto sequence(TokenCursor& tc, F const& f, Types... types) -> Expression {
  Expression expr{f(tc)};

  while (tc.match(types...)) {
    expr = Result{expr, tc.take(), f(tc)};
  }

  return expr;
}

template <typename F, typename... Types>
auto binary_expression(TokenCursor& tc, F const& f, Types... types)
    -> Expression {
  return sequence<F, BinaryExpression, Types...>(tc, f, types...);
}

template <typename F, typename... Types>
auto logical_expression(TokenCursor& tc, F const& f, Types... types)
    -> Expression {
  return sequence<F, LogicalExpression, Types...>(tc, f, types...);
}

auto factor(TokenCursor& tc) -> Expression {
  return binary_expression(tc, unary, TokenType::SLASH, TokenType::STAR);
}

auto term(TokenCursor& tc) -> Expression {
  return binary_expression(tc, factor, TokenType::MINUS, TokenType::PLUS);
}

auto comparison(TokenCursor& tc) -> Expression {
  return binary_expression(tc, term, TokenType::GREATER_EQUAL,
                           TokenType::GREATER, TokenType::LESS_EQUAL,
                           TokenType::LESS);
}

auto equality(TokenCursor& tc) -> Expression {
  return binary_expression(tc, comparison, TokenType::BANG_EQUAL,
                           TokenType::EQUAL_EQUAL);
}

auto and_expr(TokenCursor& tc) -> Expression {
  return logical_expression(tc, equality, TokenType::AND);
}

auto or_expr(TokenCursor& tc) -> Expression {
  return logical_expression(tc, and_expr, TokenType::OR);
}

auto assignment(TokenCursor& tc) -> Expression {
  Expression const expr{or_expr(tc)};

  if (tc.match(TokenType::EQUAL)) {
    Token const equals{tc.take()};
    Expression const value{or_expr(tc)};

    if (auto const var{std::get_if<VariableExpression>(&expr)}) {
      return AssignmentExpression{var->name_, value};
    }

    // Do not throw, just report the error
    error(equals, "Invalid assignment target.").report();
  }

  return expr;
}

auto expression(TokenCursor& tc) -> Expression { return assignment(tc); };

auto print_statement(TokenCursor& tc) -> Statement {
  Expression const value{expression(tc)};
  tc.consume(TokenType::SEMICOLON);
  return PrintStatement{value};
}

auto expression_statement(TokenCursor& tc) -> Statement {
  Expression const expr{expression(tc)};
  tc.consume(TokenType::SEMICOLON);
  return ExpressionStatement{expr};
}

// Forward declare declaration
auto declaration(TokenCursor& tc) -> Statement;

auto block_statement(TokenCursor& tc) -> Statement {
  std::vector<Statement> statements{};

  while (!tc.match(TokenType::RIGHT_BRACE)) {
    statements.push_back(declaration(tc));
  }

  tc.consume(TokenType::RIGHT_BRACE);

  return BlockStatement{statements};
}

// Forward declare statement
auto statement(TokenCursor& tc) -> Statement;

auto if_statement(TokenCursor& tc) -> Statement {
  tc.consume(TokenType::LEFT_PAREN);
  Expression const condition{expression(tc)};
  tc.consume(TokenType::RIGHT_PAREN);

  Statement const then_branch{statement(tc)};

  Statement else_branch{std::monostate{}};
  if (tc.match(TokenType::ELSE)) {
    tc.advance();
    else_branch = statement(tc);
  }

  return IfStatement{condition, then_branch, else_branch};
}

auto while_statement(TokenCursor& tc) -> Statement {
  tc.consume(TokenType::LEFT_PAREN);
  Expression const condition{expression(tc)};
  tc.consume(TokenType::RIGHT_PAREN);

  return WhileStatement{condition, statement(tc)};
}

// Forward declare
auto variable_declaration(TokenCursor& tc) -> Statement;
auto expression_statement(TokenCursor& tc) -> Statement;

auto for_statement(TokenCursor& tc) -> Statement {
  tc.consume(TokenType::LEFT_PAREN);

  Statement initializer{std::monostate{}};
  if (tc.match(TokenType::SEMICOLON)) {
    tc.advance();
  } else if (tc.match(TokenType::VAR)) {
    tc.advance();
    initializer = variable_declaration(tc);
  } else {
    initializer = expression_statement(tc);
  }

  Expression condition{std::monostate{}};
  if (!tc.match(TokenType::SEMICOLON)) {
    condition = expression(tc);
  }
  tc.consume(TokenType::SEMICOLON);

  Expression increment{std::monostate{}};
  if (!tc.match(TokenType::RIGHT_PAREN)) {
    increment = expression(tc);
  }
  tc.consume(TokenType::RIGHT_PAREN);

  Statement body{statement(tc)};

  if (!std::holds_alternative<std::monostate>(increment)) {
    body = BlockStatement{{body, ExpressionStatement{increment}}};
  }

  if (std::holds_alternative<std::monostate>(condition)) {
    condition = LiteralExpression{true};
  }

  body = WhileStatement{condition, body};

  if (!std::holds_alternative<std::monostate>(initializer)) {
    body = BlockStatement{{initializer, body}};
  }

  return body;
}

auto statement(TokenCursor& tc) -> Statement {
  if (tc.match(TokenType::PRINT)) {
    tc.advance();
    return print_statement(tc);
  }
  if (tc.match(TokenType::LEFT_BRACE)) {
    tc.advance();
    return block_statement(tc);
  }
  if (tc.match(TokenType::IF)) {
    tc.advance();
    return if_statement(tc);
  }
  if (tc.match(TokenType::WHILE)) {
    tc.advance();
    return while_statement(tc);
  }
  if (tc.match(TokenType::FOR)) {
    tc.advance();
    return for_statement(tc);
  }

  return expression_statement(tc);
}

auto variable_declaration(TokenCursor& tc) -> Statement {
  Token const name{tc.consume(TokenType::IDENTIFIER)};

  Expression initializer{std::monostate{}};
  if (tc.match(TokenType::EQUAL)) {
    tc.advance();
    initializer = expression(tc);
  }

  tc.consume(TokenType::SEMICOLON);

  return VariableStatement{name.lexeme_, initializer};
}

auto declaration(TokenCursor& tc) -> Statement {
  try {
    if (tc.match(TokenType::VAR)) {
      tc.advance();
      return variable_declaration(tc);
    }
    return statement(tc);
  } catch (Parser::Error const& e) {
    e.report();
    tc.synchronize();
    return std::monostate{};
  }
}
}  // namespace

namespace Parser {
auto parse(std::vector<Token const> const& tokens)
    -> std::vector<Statement const> {
  TokenCursor tc(tokens);

  std::vector<Statement const> statements{};

  while (!tc.is_at_end()) {
    statements.push_back(declaration(tc));
  }

  return statements;
}
}  // namespace Parser
