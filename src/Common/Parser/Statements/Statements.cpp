#include "./Statements.hpp"

#include <cassert>
#include <cstdint>
#include <variant>
#include <vector>

#include "../../../../submodules/RustyPtr/include/RustyPtr/Box.hpp"
#include "../../../Common/Error/Report/Report.hpp"
#include "../../../Common/Parser/Cursor/Cursor.hpp"
#include "../../../Common/Parser/Error/Error.hpp"
#include "../../../Common/Types/Tokens/Token.hpp"
#include "../../../Common/Types/Tokens/TokenTypes.hpp"
#include "../../Types/Syntax/Expression.hpp"
#include "../../Types/Syntax/Statement.hpp"
#include "../Expressions/Expressions.hpp"
#include "../Utils/Utils.hpp"

namespace LOX::Common::Parser::Statements {

using Types::Token;
using Types::Tokens::TokenType;

using Parser::Expressions::expression;
using Types::Syntax::Expressions::Expression;
using Types::Syntax::Expressions::LiteralExpr;

using Types::Syntax::Statements::Statement;

using Types::Syntax::Statements::BlockStmt;
using Types::Syntax::Statements::ClassStmt;
using Types::Syntax::Statements::ExpressionStmt;
using Types::Syntax::Statements::FunctionStmt;
using Types::Syntax::Statements::IfStmt;
using Types::Syntax::Statements::PrintStmt;
using Types::Syntax::Statements::ReturnStmt;
using Types::Syntax::Statements::VariableStmt;
using Types::Syntax::Statements::WhileStmt;

auto print_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::PRINT);

  Expression const value{expression(cursor)};

  cursor.consume(TokenType::SEMICOLON, "Expect ';' after value.");

  return PrintStmt{keyword, value};
}

auto expression_statement(Cursor& cursor) -> Statement {
  Expression const expr{expression(cursor)};
  auto const semicolon{
      cursor.consume(TokenType::SEMICOLON, "Expect ';' after expression.")};
  return ExpressionStmt{semicolon, expr};
}

auto block_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::LEFT_BRACE);

  std::vector<Statement> statements{};

  while (!cursor.match(TokenType::RIGHT_BRACE)) {
    statements.push_back(declaration(cursor));
  }

  cursor.consume(TokenType::RIGHT_BRACE, "Expect '}' to close a block.");

  return BlockStmt{keyword, statements};
}

auto if_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::IF);

  cursor.consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
  Expression const condition{expression(cursor)};
  cursor.consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

  Statement const then_branch{statement(cursor)};

  Statement else_branch{std::monostate{}};
  if (cursor.match(TokenType::ELSE)) {
    cursor.take();
    else_branch = statement(cursor);
  }

  return IfStmt{keyword, condition, then_branch, else_branch};
}

auto while_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::WHILE);

  cursor.consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
  Expression const condition{expression(cursor)};
  cursor.consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");

  return WhileStmt{keyword, condition, statement(cursor)};
}

auto for_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::FOR);

  cursor.consume(TokenType::LEFT_PAREN, "Expect '(' after 'for'.");

  Statement initializer{std::monostate{}};
  if (cursor.match(TokenType::SEMICOLON)) {
    cursor.take();
  } else if (cursor.match(TokenType::VAR)) {
    initializer = variable_declaration(cursor);
  } else {
    initializer = expression_statement(cursor);
  }

  Expression condition{std::monostate{}};
  if (!cursor.match(TokenType::SEMICOLON)) {
    condition = expression(cursor);
  }
  Token const semicolon{
      cursor.consume(TokenType::SEMICOLON, "Expect ';' after loop condition.")};

  Expression const increment{cursor.match(TokenType::RIGHT_PAREN)
                                 ? std::monostate{}
                                 : expression(cursor)};
  Token const right_paren{
      cursor.consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.")};

  Statement const body{statement(cursor)};

  return BlockStmt{
      keyword,
      {initializer,
       WhileStmt{keyword,
                 std::holds_alternative<std::monostate>(condition)
                     ? LiteralExpr{keyword.line, true}
                     : condition,
                 BlockStmt{right_paren,
                           {body, ExpressionStmt{right_paren, increment}}}}}};
}

auto return_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::RETURN);

  Expression const value{cursor.match(TokenType::SEMICOLON)
                             ? std::monostate{}
                             : expression(cursor)};
  cursor.consume(TokenType::SEMICOLON, "Expect ';' after return value.");
  return ReturnStmt{keyword, value};
}

auto statement(Cursor& cursor) -> Statement {
  if (cursor.match(TokenType::PRINT)) {
    return print_statement(cursor);
  }
  if (cursor.match(TokenType::LEFT_BRACE)) {
    return block_statement(cursor);
  }
  if (cursor.match(TokenType::IF)) {
    return if_statement(cursor);
  }
  if (cursor.match(TokenType::RETURN)) {
    return return_statement(cursor);
  }
  if (cursor.match(TokenType::WHILE)) {
    return while_statement(cursor);
  }
  if (cursor.match(TokenType::FOR)) {
    return for_statement(cursor);
  }

  return expression_statement(cursor);
}

enum class FunctionType : std::uint8_t { Function, Method };

auto function_declaration(Cursor& cursor, FunctionType type) -> Statement {
  if (type == FunctionType::Function) {
    Token const keyword{cursor.take()};
    assert(keyword.type == TokenType::FUN);
    static_cast<void>(keyword);
  }

  Token const name{
      cursor.consume(TokenType::IDENTIFIER, "Expect function name.")};

  std::vector<Token> const parameters{
      Utils::parse_parenthesized_list<Token>(cursor, [](Cursor& cursor) {
        return cursor.consume(TokenType::IDENTIFIER, "Expect parameter name.");
      })};

  std::vector<Statement> const body{block_statement(cursor)};

  return FunctionStmt{name, parameters, body};
}

auto class_declaration(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::CLASS);
  static_cast<void>(keyword);

  Token const name{cursor.consume(TokenType::IDENTIFIER, "Expect class name.")};

  Types::Syntax::Expressions::VariableExpr super_class{Token::none()};
  if (cursor.match(TokenType::LESS)) {
    cursor.take();
    super_class = Types::Syntax::Expressions::VariableExpr{
        cursor.consume(TokenType::IDENTIFIER, "Expect superclass name.")};
  }

  cursor.consume(TokenType::LEFT_BRACE, "Expect '{' before class body.");

  std::vector<Box<FunctionStmt>> methods{};

  while (!cursor.match(TokenType::RIGHT_BRACE)) {
    methods.emplace_back(*std::get<Box<FunctionStmt>>(
        function_declaration(cursor, FunctionType::Method)));
  }

  cursor.consume(TokenType::RIGHT_BRACE, "Expect '}' after class body.");

  return ClassStmt{name, super_class, methods};
}

auto variable_declaration(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::VAR);
  static_cast<void>(keyword);

  Token const name{
      cursor.consume(TokenType::IDENTIFIER, "Expect variable name.")};

  Expression initializer{std::monostate{}};
  if (cursor.match(TokenType::EQUAL)) {
    cursor.take();
    initializer = expression(cursor);
  }

  cursor.consume(TokenType::SEMICOLON,
                 "Expect ';' after variable declaration.");

  return VariableStmt{name, initializer};
}

auto declaration(Cursor& cursor) -> Statement {
  try {
    if (cursor.match(TokenType::FUN)) {
      return function_declaration(cursor, FunctionType::Function);
    }
    if (cursor.match(TokenType::CLASS)) {
      return class_declaration(cursor);
    }
    if (cursor.match(TokenType::VAR)) {
      return variable_declaration(cursor);
    }
    return statement(cursor);
  } catch (Error const& e) {
    LOX::Common::Error::report(e);
    cursor.synchronize();
    return std::monostate{};
  }
}

}  // namespace LOX::Common::Parser::Statements
