#ifndef LOX_TREEWALK_PARSER_STATEMENTS
#define LOX_TREEWALK_PARSER_STATEMENTS

#include "../../Types/Syntax/Statement.hpp"
#include "../Cursor/Cursor.hpp"

namespace LOX::TreeWalk::Parser::Statements {

auto print_statement(Cursor& cursor) -> Types::Syntax::Statements::Statement;

auto expression_statement(Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

auto block_statement(Cursor& cursor) -> Types::Syntax::Statements::Statement;

auto if_statement(Cursor& cursor) -> Types::Syntax::Statements::Statement;

auto while_statement(Cursor& cursor) -> Types::Syntax::Statements::Statement;

auto for_statement(Cursor& cursor) -> Types::Syntax::Statements::Statement;

auto return_statement(Cursor& cursor) -> Types::Syntax::Statements::Statement;

auto statement(Cursor& cursor) -> Types::Syntax::Statements::Statement;

auto function_declaration(Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

auto variable_declaration(Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

auto declaration(Cursor& cursor) -> Types::Syntax::Statements::Statement;

}  // namespace LOX::TreeWalk::Parser::Statements

#endif
