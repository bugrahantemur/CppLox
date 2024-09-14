#include "./Compiler.hpp"

#include <cstddef>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

#include "../../../submodules/RustyPtr/include/RustyPtr/Box.hpp"
#include "../../Common/Parser/Parser.hpp"
#include "../../Common/Scanner/Scanner.hpp"
#include "../../Common/Types/Syntax/Expression.hpp"
#include "../../Common/Types/Syntax/Statement.hpp"
#include "../../Common/Types/Tokens/Token.hpp"
#include "../../Common/Types/Tokens/TokenTypes.hpp"
#include "../../Common/Types/Value/Value.hpp"
#include "../../Common/Utils/Put/Put.hpp"
#include "../Chunk/Chunk.hpp"
#include "../OpCode/OpCode.hpp"
#include "../Types/Byte.hpp"

namespace LOX::ByteCode::Compiler {

using LOX::Common::Types::Token;
using LOX::Common::Types::Value;
using LOX::Common::Types::Syntax::Statements::Statement;
using LOX::Common::Types::Tokens::TokenType;

auto emit_byte(Chunk& chunk, Byte byte, std::size_t line) -> void {
  chunk.append_byte(byte, line);
}

auto emit_bytes(Chunk& chunk, Byte byte1, std::size_t line1, Byte byte2,
                std::size_t line2) -> void {
  chunk.append_byte(byte1, line1);
  chunk.append_byte(byte2, line2);
}

auto error(std::string const& msg) { throw std::runtime_error(msg); }

auto make_constant(Chunk& chunk, Value const& value) -> Byte {
  std::size_t const const_idx{chunk.append_constant(value)};
  if (const_idx > static_cast<std::size_t>(std::numeric_limits<Byte>::max())) {
    error("Too many constants in one chunk.");
    return 0;  // Unreachable
  }

  return static_cast<Byte>(const_idx);
}

auto emit_constant(Chunk& chunk, Value const& value, std::size_t line) -> void {
  emit_bytes(chunk, OpCode::CONSTANT, line, make_constant(chunk, value), line);
}

struct ExpressionCompiler {
  explicit ExpressionCompiler(Chunk& chunk) : chunk_{&chunk} {}

  auto operator()(std::monostate /*none*/) -> void {
    emit_byte(*chunk_, OpCode::NIL, 0);
  }

  auto operator()(
      Box<Common::Types::Syntax::Expressions::LiteralExpr> const& expr)
      -> void {
    if (auto const* nil{std::get_if<std::monostate>(&expr->value)}) {
      emit_byte(*chunk_, OpCode::NIL, expr->line);
    } else if (auto const* boolean{std::get_if<bool>(&expr->value)}) {
      *boolean ? emit_byte(*chunk_, OpCode::TRUE, expr->line)
               : emit_byte(*chunk_, OpCode::FALSE, expr->line);

    } else {
      emit_constant(*chunk_, expr->value, expr->line);
    }
  }

  auto operator()(
      Box<Common::Types::Syntax::Expressions::SuperExpr> const& expr) -> void {
    (void)expr;
  }

  auto operator()(Box<Common::Types::Syntax::Expressions::ThisExpr> const& expr)
      -> void {
    (void)expr;
  }

  auto operator()(
      Box<Common::Types::Syntax::Expressions::VariableExpr> const& expr)
      -> void {
    Byte const name_index{make_constant(*chunk_, expr->name.lexeme)};
    std::size_t const line{expr->name.line};
    emit_bytes(*chunk_, OpCode::GET_GLOBAL, line, name_index, line);
  }

  auto operator()(
      Box<Common::Types::Syntax::Expressions::AssignmentExpr> const& expr)
      -> void {
    // Left hand side
    Byte const name_index{make_constant(*chunk_, expr->name.lexeme)};

    // Right hand side
    std::visit(*this, expr->value);

    std::size_t const line{expr->name.line};
    emit_bytes(*chunk_, OpCode::SET_GLOBAL, line, name_index, line);
  }

  auto operator()(
      Box<Common::Types::Syntax::Expressions::BinaryExpr> const& expr) -> void {
    // Compile the operands
    std::visit(*this, expr->left);
    std::visit(*this, expr->right);

    // Emit the operator instruction
    Token const& op{expr->op};
    TokenType const op_type{op.type};
    std::size_t const op_line{op.line};

    switch (op_type) {
      case TokenType::PLUS:
        emit_byte(*chunk_, OpCode::ADD, op_line);
        break;

      case TokenType::MINUS:
        emit_byte(*chunk_, OpCode::SUBTRACT, op_line);
        break;

      case TokenType::STAR:
        emit_byte(*chunk_, OpCode::MULTIPLY, op_line);
        break;

      case TokenType::SLASH:
        emit_byte(*chunk_, OpCode::DIVIDE, op_line);
        break;

      case TokenType::EQUAL_EQUAL:
        emit_byte(*chunk_, OpCode::EQUAL, op_line);
        break;

      case TokenType::BANG_EQUAL:
        emit_byte(*chunk_, OpCode::NOT_EQUAL, op_line);
        break;

      case TokenType::LESS:
        emit_byte(*chunk_, OpCode::LESS, op_line);
        break;

      case TokenType::LESS_EQUAL:
        emit_byte(*chunk_, OpCode::LESS_EQUAL, op_line);
        break;

      case TokenType::GREATER:
        emit_byte(*chunk_, OpCode::GREATER, op_line);
        break;

      case TokenType::GREATER_EQUAL:
        emit_byte(*chunk_, OpCode::GREATER_EQUAL, op_line);
        break;

      default:
        // Handle unexpected cases
        break;
    }
  }

  auto operator()(Box<Common::Types::Syntax::Expressions::CallExpr> const& expr)
      -> void {
    (void)expr;
  }

  auto operator()(Box<Common::Types::Syntax::Expressions::GetExpr> const& expr)
      -> void {
    (void)expr;
  }

  auto operator()(
      Box<Common::Types::Syntax::Expressions::GroupingExpr> const& expr)
      -> void {
    std::visit(*this, expr->expression);
  }

  auto operator()(
      Box<Common::Types::Syntax::Expressions::LogicalExpr> const& expr)
      -> void {
    (void)expr;
  }

  auto operator()(Box<Common::Types::Syntax::Expressions::SetExpr> const& expr)
      -> void {
    (void)expr;
  }

  auto operator()(
      Box<Common::Types::Syntax::Expressions::UnaryExpr> const& expr) -> void {
    // Compile the operand
    std::visit(*this, expr->right);

    // Emit the operator instruction
    Token const& op{expr->op};
    TokenType const op_type{op.type};

    if (op_type == TokenType::MINUS) {
      emit_byte(*chunk_, OpCode::NEGATE, expr->op.line);
    } else if (op_type == TokenType::BANG) {
      emit_byte(*chunk_, OpCode::NOT, expr->op.line);
    }
  }

 private:
  Chunk* chunk_;
};

struct StatementCompiler {
  explicit StatementCompiler(Chunk& chunk) : chunk_{&chunk} {}

  auto operator()(Box<Common::Types::Syntax::Statements::PrintStmt> const& stmt)
      -> void {
    std::visit(ExpressionCompiler{*chunk_}, stmt->expression);
    emit_byte(*chunk_, OpCode::PRINT, stmt->keyword.line);
  }

  auto operator()(
      Box<Common::Types::Syntax::Statements::ExpressionStmt> const& stmt)
      -> void {
    std::visit(ExpressionCompiler{*chunk_}, stmt->expression);
    emit_byte(*chunk_, OpCode::POP, stmt->ending.line);
  }

  auto operator()(
      Box<Common::Types::Syntax::Statements::VariableStmt> const& stmt)
      -> void {
    // Left hand side
    Byte const name_index{make_constant(*chunk_, stmt->name.lexeme)};

    // Right hand side
    std::visit(ExpressionCompiler{*chunk_}, stmt->initializer);

    std::size_t const line{stmt->name.line};
    emit_bytes(*chunk_, OpCode::DEFINE_GLOBAL, line, name_index, line);
  }

  template <typename T>
  auto operator()(T const& /*unused*/) -> void {
    std::cerr << "Unhandled statement type\n";
    std::exit(1);
  }

 private:
  Chunk* chunk_;
};

auto compile(std::string const& source) -> Chunk {
  std::vector<Token> const tokens = Common::Scanner::scan(source);
  std::vector<Statement> const statements = Common::Parser::parse(tokens);

  Chunk chunk;
  for (Statement const& stmt : statements) {
    std::visit(StatementCompiler{chunk}, stmt);
  }

  return chunk;
}

}  // namespace LOX::ByteCode::Compiler
