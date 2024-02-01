#ifndef LOX_PARSER_STATEMENTS
#define LOX_PARSER_STATEMENTS

#include "../Types/Syntax/Statement.hpp"
#include "./Cursor.hpp"

namespace LOX::Parser::Statements {

auto print_statement(Cursor& cursor) -> Types::Syntax::Statement;

auto expression_statement(Cursor& cursor) -> Types::Syntax::Statement;

auto block_statement(Cursor& cursor) -> Types::Syntax::Statement;

auto if_statement(Cursor& cursor) -> Types::Syntax::Statement;

auto while_statement(Cursor& cursor) -> Types::Syntax::Statement;

auto for_statement(Cursor& cursor) -> Types::Syntax::Statement;

auto return_statement(Cursor& cursor) -> Types::Syntax::Statement;

auto statement(Cursor& cursor) -> Types::Syntax::Statement;

auto function_declaration(Cursor& cursor) -> Types::Syntax::Statement;

auto variable_declaration(Cursor& cursor) -> Types::Syntax::Statement;

auto declaration(Cursor& cursor) -> Types::Syntax::Statement;

}  // namespace LOX::Parser::Statements

#endif
