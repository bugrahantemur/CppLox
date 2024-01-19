#ifndef LOX_PARSER_STATEMENTS
#define LOX_PARSER_STATEMENTS

#include "../types/statement.hpp"
#include "./cursor.hpp"

namespace LOX::Parser::Statements {

auto print_statement(Cursor& cursor) -> Statement;

auto expression_statement(Cursor& cursor) -> Statement;

auto block_statement(Cursor& cursor) -> Statement;

auto if_statement(Cursor& cursor) -> Statement;

auto while_statement(Cursor& cursor) -> Statement;

auto for_statement(Cursor& cursor) -> Statement;

auto return_statement(Cursor& cursor) -> Statement;

auto statement(Cursor& cursor) -> Statement;

auto function_declaration(Cursor& cursor) -> Statement;

auto variable_declaration(Cursor& cursor) -> Statement;

auto declaration(Cursor& cursor) -> Statement;
}  // namespace LOX::Parser::Statements

#endif
