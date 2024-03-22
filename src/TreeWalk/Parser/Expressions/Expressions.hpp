#ifndef LOX_TREEWALK_PARSER_EXPRESSIONS
#define LOX_TREEWALK_PARSER_EXPRESSIONS

#include "../../../Common/Parser/Cursor/Cursor.hpp"
#include "../../Types/Syntax/Expression.hpp"

namespace LOX::TreeWalk::Parser::Expressions {

auto primary(Common::Parser::Cursor& tc)
    -> Types::Syntax::Expressions::Expression;

auto call(Common::Parser::Cursor& tc) -> Types::Syntax::Expressions::Expression;

auto unary(Common::Parser::Cursor& tc)
    -> Types::Syntax::Expressions::Expression;

template <typename F, typename Result, typename... Ts>
auto sequence(Common::Parser::Cursor& tc, F const& f, Ts... ts)
    -> Types::Syntax::Expressions::Expression;

template <typename F, typename... Ts>
auto binary_expression(Common::Parser::Cursor& tc, F const& f, Ts... ts)
    -> Types::Syntax::Expressions::Expression;

template <typename F, typename... Ts>
auto logical_expression(Common::Parser::Cursor& tc, F const& f, Ts... ts)
    -> Types::Syntax::Expressions::Expression;

auto factor(Common::Parser::Cursor& tc)
    -> Types::Syntax::Expressions::Expression;

auto term(Common::Parser::Cursor& tc) -> Types::Syntax::Expressions::Expression;

auto comparison(Common::Parser::Cursor& tc)
    -> Types::Syntax::Expressions::Expression;

auto equality(Common::Parser::Cursor& tc)
    -> Types::Syntax::Expressions::Expression;

auto and_expr(Common::Parser::Cursor& tc)
    -> Types::Syntax::Expressions::Expression;

auto or_expr(Common::Parser::Cursor& tc)
    -> Types::Syntax::Expressions::Expression;

auto assignment(Common::Parser::Cursor& tc)
    -> Types::Syntax::Expressions::Expression;

auto expression(Common::Parser::Cursor& tc)
    -> Types::Syntax::Expressions::Expression;

}  // namespace LOX::TreeWalk::Parser::Expressions

#endif
