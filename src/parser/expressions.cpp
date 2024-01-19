#include "./expressions.hpp"

#include "./cursor.hpp"
#include "./error.hpp"
#include "./utils.hpp"

namespace LOX::Parser::Expressions {

auto primary(Cursor& cursor) -> Expression {
  if (cursor.match(TokenType::FALSE)) {
    cursor.take();
    return LiteralExpression{false};
  }
  if (cursor.match(TokenType::TRUE)) {
    cursor.take();
    return LiteralExpression{true};
  }
  if (cursor.match(TokenType::NIL)) {
    cursor.take();
    return LiteralExpression{std::monostate{}};
  }
  if (cursor.match(TokenType::NUMBER, TokenType::STRING)) {
    return LiteralExpression{cursor.take().literal_};
  }
  if (cursor.match(TokenType::LEFT_PAREN)) {
    cursor.take();
    Expression const expr{expression(cursor)};
    cursor.take(TokenType::RIGHT_PAREN);
    return GroupingExpression{expr};
  }
  if (cursor.match(TokenType::IDENTIFIER)) {
    return VariableExpression{cursor.take()};
  }
  if (cursor.match(TokenType::THIS)) {
    return ThisExpression{cursor.take()};
  }
  if (cursor.match(TokenType::SUPER)) {
    Token const keyword{cursor.take()};
    cursor.take(TokenType::DOT);
    Token const method{cursor.take(TokenType::IDENTIFIER)};
    return SuperExpression{keyword, method};
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

      expr = CallExpression{expr, closing, args};
    } else if (cursor.match(TokenType::DOT)) {
      Token const dot{cursor.take()};
      static_cast<void>(dot);

      Token const name{cursor.take(TokenType::IDENTIFIER)};
      expr = Box{GetExpression{name, expr}};
    } else {
      break;
    }
  }

  return expr;
}

auto unary(Cursor& cursor) -> Expression {
  if (cursor.match(TokenType::BANG, TokenType::MINUS)) {
    Token const op{cursor.take()};
    return UnaryExpression{op, unary(cursor)};
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
  return sequence<F, BinaryExpression, Types...>(cursor, f, types...);
}

template <typename F, typename... Types>
auto logical_expression(Cursor& cursor, F const& f, Types... types)
    -> Expression {
  return sequence<F, LogicalExpression, Types...>(cursor, f, types...);
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

    if (auto const var{std::get_if<Box<VariableExpression>>(&expr)}) {
      return AssignmentExpression{(*var)->name_, value};
    } else if (auto const get{std::get_if<Box<GetExpression>>(&expr)}) {
      return SetExpression{(*get)->name_, (*get)->object_, value};
    }

    // Do not throw, just report the error
    error(equals, "Invalid assignment target.").report();
  }

  return expr;
}

auto expression(Cursor& cursor) -> Expression { return assignment(cursor); };

}  // namespace LOX::Parser::Expressions
