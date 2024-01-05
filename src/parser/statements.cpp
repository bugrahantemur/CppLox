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
  static_cast<void>(keyword);

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
  static_cast<void>(keyword);

  std::vector<Statement> statements{};

  while (!cursor.match(TokenType::RIGHT_BRACE)) {
    statements.push_back(declaration(cursor));
  }

  cursor.take(TokenType::RIGHT_BRACE);

  return Box{BlockStatement{statements}};
}

auto if_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type_ == TokenType::IF);
  static_cast<void>(keyword);

  cursor.take(TokenType::LEFT_PAREN);
  Expression const condition{Expressions::expression(cursor)};
  cursor.take(TokenType::RIGHT_PAREN);

  Statement const then_branch{statement(cursor)};

  Statement else_branch{std::monostate{}};
  if (cursor.match(TokenType::ELSE)) {
    cursor.take();
    else_branch = statement(cursor);
  }

  return Box{IfStatement{condition, then_branch, else_branch}};
}

auto while_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type_ == TokenType::WHILE);
  static_cast<void>(keyword);

  cursor.take(TokenType::LEFT_PAREN);
  Expression const condition{Expressions::expression(cursor)};
  cursor.take(TokenType::RIGHT_PAREN);

  return Box{WhileStatement{condition, statement(cursor)}};
}

auto for_statement(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type_ == TokenType::FOR);
  static_cast<void>(keyword);

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

  Expression const increment{cursor.match(TokenType::RIGHT_PAREN)
                                 ? std::monostate{}
                                 : Expressions::expression(cursor)};
  cursor.take(TokenType::RIGHT_PAREN);

  Statement const body{statement(cursor)};

  return Box{BlockStatement{
      {initializer,
       WhileStatement{
           std::holds_alternative<std::monostate>(condition)
               ? LiteralExpression{true}
               : condition,
           BlockStatement{{body, ExpressionStatement{increment}}}}}}};
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

auto parse_params(Cursor& cursor) -> std::vector<Token> {
  return Utils::parse_parenthesized_list<Token>(cursor, [](Cursor& cursor) {
    return cursor.take(TokenType::IDENTIFIER);
  });
}

enum class FunctionType { Function, Method };

auto function_declaration(Cursor& cursor, FunctionType type) -> Statement {
  if (type == FunctionType::Function) {
    Token const keyword{cursor.take()};
    assert(keyword.type_ == TokenType::FUN);
    static_cast<void>(keyword);
  }

  Token const name{cursor.take(TokenType::IDENTIFIER)};

  std::vector<Token> const parameters{parse_params(cursor)};

  std::vector<Statement> const body{block_statement(cursor)};

  return Box{FunctionStatement{name, parameters, body}};
}

auto class_declaration(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type_ == TokenType::CLASS);
  static_cast<void>(keyword);

  Token const name{cursor.take(TokenType::IDENTIFIER)};

  cursor.take(TokenType::LEFT_BRACE);

  std::vector<Box<FunctionStatement>> methods{};

  while (!cursor.match(TokenType::RIGHT_BRACE)) {
    methods.emplace_back(*std::get<Box<FunctionStatement>>(
        function_declaration(cursor, FunctionType::Method)));
  }

  cursor.take(TokenType::RIGHT_BRACE);

  return Box{ClassStatement{name, methods}};
}

auto variable_declaration(Cursor& cursor) -> Statement {
  Token const keyword{cursor.take()};
  assert(keyword.type_ == TokenType::VAR);
  static_cast<void>(keyword);

  Token const name{cursor.take(TokenType::IDENTIFIER)};

  Expression initializer{std::monostate{}};
  if (cursor.match(TokenType::EQUAL)) {
    cursor.take();
    initializer = Expressions::expression(cursor);
  }

  cursor.take(TokenType::SEMICOLON);

  return VariableStatement{name, initializer};
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
    e.report();
    cursor.synchronize();
    return std::monostate{};
  }
}
}  // namespace Parser::Statements
