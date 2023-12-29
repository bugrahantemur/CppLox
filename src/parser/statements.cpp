#include "./statements.hpp"

#include "../types/expression.hpp"
#include "../types/statement.hpp"
#include "./cursor.hpp"
#include "./error.hpp"
#include "./expressions.hpp"
#include "./utils.hpp"

namespace Parser::Statements {
auto print_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type_ == TokenType::PRINT);

  Expression const value{Expressions::expression(cursor)};

  cursor.take(TokenType::SEMICOLON);

  return PrintStatement{value};
}

auto expression_statement(Cursor& cursor) -> Statement {
  Expression const expr{Expressions::expression(cursor)};
  cursor.take(TokenType::SEMICOLON);
  return ExpressionStatement{expr};
}

auto block_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type_ == TokenType::LEFT_BRACE);

  std::vector<Statement> statements{};

  while (!cursor.match(TokenType::RIGHT_BRACE)) {
    statements.push_back(declaration(cursor));
  }

  cursor.take(TokenType::RIGHT_BRACE);

  return BlockStatement{statements};
}

auto if_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type_ == TokenType::IF);

  cursor.take(TokenType::LEFT_PAREN);
  Expression const condition{Expressions::expression(cursor)};
  cursor.take(TokenType::RIGHT_PAREN);

  Statement const then_branch{statement(cursor)};

  Statement else_branch{std::monostate{}};
  if (cursor.match(TokenType::ELSE)) {
    cursor.take();
    else_branch = statement(cursor);
  }

  return IfStatement{condition, then_branch, else_branch};
}

auto while_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type_ == TokenType::WHILE);

  cursor.take(TokenType::LEFT_PAREN);
  Expression const condition{Expressions::expression(cursor)};
  cursor.take(TokenType::RIGHT_PAREN);

  return WhileStatement{condition, statement(cursor)};
}

auto for_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type_ == TokenType::FOR);

  cursor.take(TokenType::LEFT_PAREN);

  Statement initializer{std::monostate{}};
  if (cursor.match(TokenType::SEMICOLON)) {
    cursor.take();
  } else if (cursor.match(TokenType::VAR)) {
    cursor.take();
    initializer = variable_declaration(cursor);
  } else {
    initializer = expression_statement(cursor);
  }

  Expression condition{std::monostate{}};
  if (!cursor.match(TokenType::SEMICOLON)) {
    condition = Expressions::expression(cursor);
  }
  cursor.take(TokenType::SEMICOLON);

  Expression increment{std::monostate{}};
  if (!cursor.match(TokenType::RIGHT_PAREN)) {
    increment = Expressions::expression(cursor);
  }
  cursor.take(TokenType::RIGHT_PAREN);

  Statement body{statement(cursor)};

  if (!std::holds_alternative<std::monostate>(increment)) {
    body = BlockStatement{{body, ExpressionStatement{increment}}};
  }

  if (std::holds_alternative<std::monostate>(condition)) {
    condition = LiteralExpression{true};
  }

  body = WhileStatement{condition, body};

  if (!std::holds_alternative<std::monostate>(initializer)) {
    body = BlockStatement{{initializer, body}};
  }

  return body;
}

auto return_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type_ == TokenType::RETURN);

  Expression const value{cursor.match(TokenType::SEMICOLON)
                             ? std::monostate{}
                             : Expressions::expression(cursor)};
  cursor.take(TokenType::SEMICOLON);
  return ReturnStatement{keyword, value};
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

auto parse_params(Cursor& cursor) -> std::vector<std::string> {
  return Utils::parse_parenthesized_list<std::string>(
      cursor, [](Cursor& cursor) {
        return cursor.take(TokenType::IDENTIFIER).lexeme_;
      });
}

auto function_declaration(Cursor& cursor) -> Statement {
  Token const name{cursor.take(TokenType::IDENTIFIER)};

  std::vector<std::string> const parameters{parse_params(cursor)};

  std::vector<Statement> const body{block_statement(cursor)};

  return FunctionStatement{name.lexeme_, parameters, body};
}

auto variable_declaration(Cursor& cursor) -> Statement {
  Token const name{cursor.take(TokenType::IDENTIFIER)};

  Expression initializer{std::monostate{}};
  if (cursor.match(TokenType::EQUAL)) {
    cursor.take();
    initializer = Expressions::expression(cursor);
  }

  cursor.take(TokenType::SEMICOLON);

  return VariableStatement{name.lexeme_, initializer};
}

auto declaration(Cursor& cursor) -> Statement {
  try {
    if (cursor.match(TokenType::FUN)) {
      cursor.take();
      return function_declaration(cursor);
    }
    if (cursor.match(TokenType::VAR)) {
      cursor.take();
      return variable_declaration(cursor);
    }
    return statement(cursor);
  } catch (Error const& e) {
    e.report();
    cursor.synchronize();
    return std::monostate{};
  }
}
}  // namespace Parser::Statements
