#include "./expressions.hpp"

#include <iostream>

#include "../syntax_types/expression.hpp"
#include "../utils/error.hpp"
#include "./Error/Error.hpp"
#include "./cursor.hpp"
#include "./utils.hpp"

namespace LOX::Parser::Expressions {

using namespace LOX::Expressions;

auto primary(Cursor& cursor) -> Expression {
  if (cursor.match(TokenType::FALSE)) {
    cursor.take();
    return LiteralExpr{false};
  }
  if (cursor.match(TokenType::TRUE)) {
    cursor.take();
    return LiteralExpr{true};
  }
  if (cursor.match(TokenType::NIL)) {
    cursor.take();
    return LiteralExpr{std::monostate{}};
  }
  if (cursor.match(TokenType::NUMBER, TokenType::STRING)) {
    return LiteralExpr{cursor.take().literal_};
  }
  if (cursor.match(TokenType::LEFT_PAREN)) {
    cursor.take();
    Expression const expr{expression(cursor)};
    cursor.consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
    return GroupingExpr{expr};
  }
  if (cursor.match(TokenType::IDENTIFIER)) {
    return VariableExpr{cursor.take()};
  }
  if (cursor.match(TokenType::THIS)) {
    return ThisExpr{cursor.take()};
  }
  if (cursor.match(TokenType::SUPER)) {
    Token const keyword{cursor.take()};
    cursor.consume(TokenType::DOT, "Expect '.' after 'super'.");
    Token const method{cursor.consume(TokenType::IDENTIFIER,
                                      "Expect superclass method name.")};
    return SuperExpr{keyword, method};
  }

  throw error(cursor.peek(), "Expected expression.");
}

auto parse_args(Cursor& cursor) -> std::vector<Expression> {
  return Utils::parse_parenthesized_list<Expression>(cursor, expression);
}

auto call(Cursor& cursor) -> Expression {
  Expression expr{primary(cursor)};

  while (true) {
    if (cursor.match(TokenType::LEFT_PAREN)) {
      std::vector<Expression> const args{parse_args(cursor)};

      Token const closing{cursor.previous()};
      assert(closing.type_ == TokenType::RIGHT_PAREN);

      expr = CallExpr{expr, closing, args};
    } else if (cursor.match(TokenType::DOT)) {
      Token const dot{cursor.take()};
      static_cast<void>(dot);

      Token const name{
          cursor.consume(TokenType::IDENTIFIER, "Expect property name.")};
      expr = Box{GetExpr{name, expr}};
    } else {
      break;
    }
  }

  return expr;
}

auto unary(Cursor& cursor) -> Expression {
  if (cursor.match(TokenType::BANG, TokenType::MINUS)) {
    Token const op{cursor.take()};
    return UnaryExpr{op, unary(cursor)};
  }

  return call(cursor);
}

template <typename F, typename Result, typename... Types>
auto sequence(Cursor& cursor, F const& f, Types... types) -> Expression {
  Expression expr{f(cursor)};

  while (cursor.match(types...)) {
    expr = Result{expr, cursor.take(), f(cursor)};
  }

  return expr;
}

template <typename F, typename... Types>
auto binary_expression(Cursor& cursor, F const& f, Types... types)
    -> Expression {
  return sequence<F, BinaryExpr, Types...>(cursor, f, types...);
}

template <typename F, typename... Types>
auto logical_expression(Cursor& cursor, F const& f, Types... types)
    -> Expression {
  return sequence<F, LogicalExpr, Types...>(cursor, f, types...);
}

auto factor(Cursor& cursor) -> Expression {
  return binary_expression(cursor, unary, TokenType::SLASH, TokenType::STAR);
}

auto term(Cursor& cursor) -> Expression {
  return binary_expression(cursor, factor, TokenType::MINUS, TokenType::PLUS);
}

auto comparison(Cursor& cursor) -> Expression {
  return binary_expression(cursor, term, TokenType::GREATER_EQUAL,
                           TokenType::GREATER, TokenType::LESS_EQUAL,
                           TokenType::LESS);
}

auto equality(Cursor& cursor) -> Expression {
  return binary_expression(cursor, comparison, TokenType::BANG_EQUAL,
                           TokenType::EQUAL_EQUAL);
}

auto and_expr(Cursor& cursor) -> Expression {
  return logical_expression(cursor, equality, TokenType::AND);
}

auto or_expr(Cursor& cursor) -> Expression {
  return logical_expression(cursor, and_expr, TokenType::OR);
}

auto assignment(Cursor& cursor) -> Expression {
  Expression const expr{or_expr(cursor)};

  if (cursor.match(TokenType::EQUAL)) {
    Token const equals{cursor.take()};
    Expression const value{or_expr(cursor)};

    if (auto const var{std::get_if<Box<VariableExpr>>(&expr)}) {
      return AssignmentExpr{(*var)->name_, value};
    } else if (auto const get{std::get_if<Box<GetExpr>>(&expr)}) {
      return SetExpr{(*get)->name_, (*get)->object_, value};
    }

    // Do not throw, just report the error
    LOX::report(error(equals, "Invalid assignment target."));
  }

  return expr;
}

auto expression(Cursor& cursor) -> Expression { return assignment(cursor); };

}  // namespace LOX::Parser::Expressions
