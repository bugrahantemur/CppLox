#include "./Statements.hpp"

#include <iostream>

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

using namespace LOX::Common::Types::Tokens;
using LOX::Common::Types::Token;

using namespace LOX::Common::Types::Syntax::Statements;
using namespace LOX::Common::Types::Syntax::Expressions;

auto print_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::PRINT);
  static_cast<void>(keyword);

  Expression const value{Expressions::expression(cursor)};

  cursor.consume(TokenType::SEMICOLON, "Expect ';' after value.");

  return PrintStmt{value};
}

auto expression_statement(Cursor& cursor) -> Statement {
  Expression const expr{Expressions::expression(cursor)};
  cursor.consume(TokenType::SEMICOLON, "Expect ';' after expression.");
  return ExpressionStmt{expr};
}

auto block_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::LEFT_BRACE);
  static_cast<void>(keyword);

  std::vector<Statement> statements{};

  while (!cursor.match(TokenType::RIGHT_BRACE)) {
    statements.push_back(declaration(cursor));
  }

  cursor.consume(TokenType::RIGHT_BRACE, "Expect '}' to close a block.");

  return BlockStmt{statements};
}

auto if_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::IF);
  static_cast<void>(keyword);

  cursor.consume(TokenType::LEFT_PAREN, "Expect '(' after 'if'.");
  Expression const condition{Expressions::expression(cursor)};
  cursor.consume(TokenType::RIGHT_PAREN, "Expect ')' after if condition.");

  Statement const then_branch{statement(cursor)};

  Statement else_branch{std::monostate{}};
  if (cursor.match(TokenType::ELSE)) {
    cursor.take();
    else_branch = statement(cursor);
  }

  return IfStmt{condition, then_branch, else_branch};
}

auto while_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::WHILE);
  static_cast<void>(keyword);

  cursor.consume(TokenType::LEFT_PAREN, "Expect '(' after 'while'.");
  Expression const condition{Expressions::expression(cursor)};
  cursor.consume(TokenType::RIGHT_PAREN, "Expect ')' after condition.");

  return WhileStmt{condition, statement(cursor)};
}

auto for_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::FOR);
  static_cast<void>(keyword);

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
    condition = Expressions::expression(cursor);
  }
  cursor.consume(TokenType::SEMICOLON, "Expect ';' after loop condition.");

  Expression const increment{cursor.match(TokenType::RIGHT_PAREN)
                                 ? std::monostate{}
                                 : Expressions::expression(cursor)};
  cursor.consume(TokenType::RIGHT_PAREN, "Expect ')' after for clauses.");

  Statement const body{statement(cursor)};

  return BlockStmt{
      {initializer,
       WhileStmt{
           std::holds_alternative<std::monostate>(condition)
               ? Types::Syntax::Expressions::LiteralExpr{keyword.line, true}
               : condition,
           BlockStmt{{body, ExpressionStmt{increment}}}}}};
}

auto return_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type == TokenType::RETURN);

  Expression const value{cursor.match(TokenType::SEMICOLON)
                             ? std::monostate{}
                             : Expressions::expression(cursor)};
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

enum class FunctionType { Function, Method };

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
    initializer = Expressions::expression(cursor);
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
