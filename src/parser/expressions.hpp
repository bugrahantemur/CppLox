#ifndef LOX_PARSER_EXPRESSIONS
#define LOX_PARSER_EXPRESSIONS

#include "../types/expression.hpp"
#include "./cursor.hpp"

namespace LOX::Parser::Expressions {

auto primary(Cursor& tc) -> Expression;

auto parse_args(Cursor& tc) -> std::vector<Expression>;

auto call(Cursor& tc) -> Expression;

auto unary(Cursor& tc) -> Expression;

template <typename F, typename Result, typename... Types>
auto sequence(Cursor& tc, F const& f, Types... types) -> Expression;

template <typename F, typename... Types>
auto binary_expression(Cursor& tc, F const& f, Types... types) -> Expression;

template <typename F, typename... Types>
auto logical_expression(Cursor& tc, F const& f, Types... types) -> Expression;

auto factor(Cursor& tc) -> Expression;

auto term(Cursor& tc) -> Expression;

auto comparison(Cursor& tc) -> Expression;

auto equality(Cursor& tc) -> Expression;

auto and_expr(Cursor& tc) -> Expression;

auto or_expr(Cursor& tc) -> Expression;

auto assignment(Cursor& tc) -> Expression;

auto expression(Cursor& tc) -> Expression;

}  // namespace LOX::Parser::Expressions

#endif
