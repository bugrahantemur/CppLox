#include "./Expressions.hpp"

#include <cassert>
#include <initializer_list>
#include <variant>
#include <vector>

#include "../../../../submodules/RustyPtr/include/RustyPtr/Box.hpp"
#include "../../../Common/Error/Report/Report.hpp"
#include "../../../Common/Parser/Cursor/Cursor.hpp"
#include "../../../Common/Parser/Error/Error.hpp"
#include "../../../Common/Types/Tokens/Token.hpp"
#include "../../../Common/Types/Tokens/TokenTypes.hpp"
#include "../../Types/Syntax/Expression.hpp"
#include "../Utils/Utils.hpp"

namespace LOX::Common::Parser::Expressions {

using Types::Token;
using Types::Tokens::TokenType;

using Types::Syntax::Expressions::Expression;

using Types::Syntax::Expressions::AssignmentExpr;
using Types::Syntax::Expressions::BinaryExpr;
using Types::Syntax::Expressions::CallExpr;
using Types::Syntax::Expressions::GetExpr;
using Types::Syntax::Expressions::GroupingExpr;
using Types::Syntax::Expressions::LiteralExpr;
using Types::Syntax::Expressions::LogicalExpr;
using Types::Syntax::Expressions::SetExpr;
using Types::Syntax::Expressions::SuperExpr;
using Types::Syntax::Expressions::ThisExpr;
using Types::Syntax::Expressions::UnaryExpr;
using Types::Syntax::Expressions::VariableExpr;

auto primary(Cursor& cursor) -> Expression {
  if (cursor.match(TokenType::FALSE)) {
    return LiteralExpr{cursor.take().line, false};
  }
  if (cursor.match(TokenType::TRUE)) {
    return LiteralExpr{cursor.take().line, true};
  }
  if (cursor.match(TokenType::NIL)) {
    return LiteralExpr{cursor.take().line, std::monostate{}};
  }
  if (cursor.match_any_of({TokenType::NUMBER, TokenType::STRING})) {
    Token const token{cursor.take()};
    return LiteralExpr{token.line, token.literal};
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

auto call(Cursor& cursor) -> Expression {
  Expression expr{primary(cursor)};

  while (true) {
    if (cursor.match(TokenType::LEFT_PAREN)) {
      std::vector<Expression> const args{
          Utils::parse_parenthesized_list<Expression>(cursor, expression)};

      Token const closing{cursor.previous()};
      assert(closing.type == TokenType::RIGHT_PAREN);

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
  if (cursor.match_any_of({TokenType::BANG, TokenType::MINUS})) {
    Token const op{cursor.take()};
    return UnaryExpr{op, unary(cursor)};
  }

  return call(cursor);
}

template <typename F, typename Result>
auto sequence(Cursor& cursor, F const& f,
              std::initializer_list<TokenType> const& types) -> Expression {
  Expression expr{f(cursor)};

  while (cursor.match_any_of(types)) {
    expr = Result{expr, cursor.take(), f(cursor)};
  }

  return expr;
}

template <typename F>
auto binary_expression(Cursor& cursor, F const& f,
                       std::initializer_list<TokenType> const& types)
    -> Expression {
  return sequence<F, BinaryExpr>(cursor, f, types);
}

template <typename F>
auto logical_expression(Cursor& cursor, F const& f, TokenType const& type)
    -> Expression {
  return sequence<F, LogicalExpr>(cursor, f, {type});
}

auto factor(Cursor& cursor) -> Expression {
  return binary_expression(cursor, unary, {TokenType::SLASH, TokenType::STAR});
}

auto term(Cursor& cursor) -> Expression {
  return binary_expression(cursor, factor, {TokenType::MINUS, TokenType::PLUS});
}

auto comparison(Cursor& cursor) -> Expression {
  return binary_expression(cursor, term,
                           {TokenType::GREATER_EQUAL, TokenType::GREATER,
                            TokenType::LESS_EQUAL, TokenType::LESS});
}

auto equality(Cursor& cursor) -> Expression {
  return binary_expression(cursor, comparison,
                           {TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL});
}

auto and_expr(Cursor& cursor) -> Expression {
  return logical_expression(cursor, equality, TokenType::AND);
}

auto or_expr(Cursor& cursor) -> Expression {
  return logical_expression(cursor, and_expr, TokenType::OR);
}

auto assignment(Cursor& cursor) -> Expression {
  Expression expr{or_expr(cursor)};

  if (cursor.match(TokenType::EQUAL)) {
    Token const equals{cursor.take()};
    Expression const value{or_expr(cursor)};

    if (auto const* const var{std::get_if<Box<VariableExpr>>(&expr)}) {
      return AssignmentExpr{(*var)->name, value};
    }
    if (auto const* const get{std::get_if<Box<GetExpr>>(&expr)}) {
      return SetExpr{(*get)->name, (*get)->object, value};
    }

    // Do not throw, just report the error
    Common::Error::report(error(equals, "Invalid assignment target."));
  }

  return expr;
}

auto expression(Cursor& cursor) -> Expression { return assignment(cursor); };

}  // namespace LOX::Common::Parser::Expressions
