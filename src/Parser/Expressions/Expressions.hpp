#ifndef LOX_PARSER_EXPRESSIONS
#define LOX_PARSER_EXPRESSIONS

#include "../../Types/Syntax/Expression.hpp"
#include "../Cursor/Cursor.hpp"

namespace LOX::Parser::Expressions {

auto primary(Cursor& tc) -> Types::Syntax::Expressions::Expression;

auto parse_args(Cursor& tc)
    -> std::vector<Types::Syntax::Expressions::Expression>;

auto call(Cursor& tc) -> Types::Syntax::Expressions::Expression;

auto unary(Cursor& tc) -> Types::Syntax::Expressions::Expression;

template <typename F, typename Result, typename... Ts>
auto sequence(Cursor& tc, F const& f, Ts... ts)
    -> Types::Syntax::Expressions::Expression;

template <typename F, typename... Ts>
auto binary_expression(Cursor& tc, F const& f, Ts... ts)
    -> Types::Syntax::Expressions::Expression;

template <typename F, typename... Ts>
auto logical_expression(Cursor& tc, F const& f, Ts... ts)
    -> Types::Syntax::Expressions::Expression;

auto factor(Cursor& tc) -> Types::Syntax::Expressions::Expression;

auto term(Cursor& tc) -> Types::Syntax::Expressions::Expression;

auto comparison(Cursor& tc) -> Types::Syntax::Expressions::Expression;

auto equality(Cursor& tc) -> Types::Syntax::Expressions::Expression;

auto and_expr(Cursor& tc) -> Types::Syntax::Expressions::Expression;

auto or_expr(Cursor& tc) -> Types::Syntax::Expressions::Expression;

auto assignment(Cursor& tc) -> Types::Syntax::Expressions::Expression;

auto expression(Cursor& tc) -> Types::Syntax::Expressions::Expression;

}  // namespace LOX::Parser::Expressions

#endif
