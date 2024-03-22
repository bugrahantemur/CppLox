#ifndef LOX_TREEWALK_PARSER_STATEMENTS
#define LOX_TREEWALK_PARSER_STATEMENTS

#include "../../../Common/Parser/Cursor/Cursor.hpp"
#include "../../Types/Syntax/Statement.hpp"

namespace LOX::TreeWalk::Parser::Statements {

auto print_statement(Common::Parser::Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

auto expression_statement(Common::Parser::Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

auto block_statement(Common::Parser::Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

auto if_statement(Common::Parser::Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

auto while_statement(Common::Parser::Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

auto for_statement(Common::Parser::Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

auto return_statement(Common::Parser::Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

auto statement(Common::Parser::Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

auto function_declaration(Common::Parser::Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

auto variable_declaration(Common::Parser::Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

auto declaration(Common::Parser::Cursor& cursor)
    -> Types::Syntax::Statements::Statement;

}  // namespace LOX::TreeWalk::Parser::Statements

#endif
