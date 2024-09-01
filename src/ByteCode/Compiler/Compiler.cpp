#include "./Compiler.hpp"

#include <limits>
#include <stdexcept>
#include <variant>

#include "../../Common/Parser/Parser.hpp"
#include "../../Common/Scanner/Scanner.hpp"
#include "../../Common/Types/Syntax/Expression.hpp"
#include "../../Common/Types/Syntax/Statement.hpp"
#include "../../Common/Types/Tokens/Token.hpp"
#include "../../Common/Types/Tokens/TokenTypes.hpp"
#include "../../Common/Types/Value/Value.hpp"
#include "../Chunk/Chunk.hpp"
#include "../OpCode/OpCode.hpp"

namespace LOX::ByteCode::Compiler {

using LOX::Common::Types::Token;
using LOX::Common::Types::Value;
using LOX::Common::Types::Syntax::Statements::Statement;
using LOX::Common::Types::Tokens::TokenType;
using namespace LOX::Common::Types::Syntax::Expressions;

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
  std::size_t const_idx{chunk.append_constant(value)};
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
  auto operator()(std::monostate none) -> void {
    (void)none;

    emit_byte(chunk, OpCode::NIL, 0);
  }

  auto operator()(Box<LiteralExpr> const& expr) -> void {
    if (auto const* nil{std::get_if<std::monostate>(&expr->value)}) {
      emit_byte(chunk, OpCode::NIL, expr->line);

    } else if (auto const* boolean{std::get_if<bool>(&expr->value)}) {
      *boolean ? emit_byte(chunk, OpCode::TRUE, expr->line)
               : emit_byte(chunk, OpCode::FALSE, expr->line);

    } else {
      emit_constant(chunk, expr->value, expr->line);
    }
  }

  auto operator()(Box<SuperExpr> const& expr) -> void { (void)expr; }

  auto operator()(Box<ThisExpr> const& expr) -> void { (void)expr; }

  auto operator()(Box<VariableExpr> const& expr) -> void { (void)expr; }

  auto operator()(Box<AssignmentExpr> const& expr) -> void { (void)expr; }

  auto operator()(Box<BinaryExpr> const& expr) -> void {
    // Compile the operands
    std::visit(*this, expr->left);
    std::visit(*this, expr->right);

    // Emit the operator instruction
    Token const& op{expr->op};
    TokenType const op_type{op.type};
    std::size_t const op_line{op.line};

    switch (op_type) {
      case TokenType::PLUS:
        emit_byte(chunk, OpCode::ADD, op_line);
        break;

      case TokenType::MINUS:
        emit_byte(chunk, OpCode::SUBTRACT, op_line);
        break;

      case TokenType::STAR:
        emit_byte(chunk, OpCode::MULTIPLY, op_line);
        break;

      case TokenType::SLASH:
        emit_byte(chunk, OpCode::DIVIDE, op_line);
        break;

      case TokenType::EQUAL_EQUAL:
        emit_byte(chunk, OpCode::EQUAL, op_line);
        break;

      case TokenType::BANG_EQUAL:
        emit_byte(chunk, OpCode::NOT_EQUAL, op_line);
        break;

      case TokenType::LESS:
        emit_byte(chunk, OpCode::LESS, op_line);
        break;

      case TokenType::LESS_EQUAL:
        emit_byte(chunk, OpCode::LESS_EQUAL, op_line);
        break;

      case TokenType::GREATER:
        emit_byte(chunk, OpCode::GREATER, op_line);
        break;

      case TokenType::GREATER_EQUAL:
        emit_byte(chunk, OpCode::GREATER_EQUAL, op_line);
        break;

      default:
        // Handle unexpected cases
        break;
    }
  }

  auto operator()(Box<CallExpr> const& expr) -> void { (void)expr; }

  auto operator()(Box<GetExpr> const& expr) -> void { (void)expr; }

  auto operator()(Box<GroupingExpr> const& expr) -> void {
    std::visit(*this, expr->expression);
  }

  auto operator()(Box<LogicalExpr> const& expr) -> void { (void)expr; }

  auto operator()(Box<SetExpr> const& expr) -> void { (void)expr; }

  auto operator()(Box<UnaryExpr> const& expr) -> void {
    // Compile the operand
    std::visit(*this, expr->right);

    // Emit the operator instruction
    Token const& op{expr->op};
    TokenType const op_type{op.type};

    if (op_type == TokenType::MINUS) {
      emit_byte(chunk, OpCode::NEGATE, expr->op.line);
    } else if (op_type == TokenType::BANG) {
      emit_byte(chunk, OpCode::NOT, expr->op.line);
    }
  }

  Chunk& chunk;
};

struct StatementCompiler {
  auto operator()(Box<Common::Types::Syntax::Statements::PrintStmt> const& stmt)
      -> void {
    std::visit(ExpressionCompiler{chunk}, stmt->expression);
    emit_byte(chunk, OpCode::PRINT, stmt->keyword.line);
  }

  auto operator()(
      Box<Common::Types::Syntax::Statements::ExpressionStmt> const& stmt)
      -> void {
    std::visit(ExpressionCompiler{chunk}, stmt->expression);
    emit_byte(chunk, OpCode::POP, stmt->ending.line);
  }

  auto operator()(
      Box<Common::Types::Syntax::Statements::VariableStmt> const& stmt)
      -> void {
    // Left hand side
    Byte const name_index{make_constant(chunk, stmt->name.lexeme)};

    // Right hand side
    std::visit(*this, stmt->initializer);

    std::size_t const line{stmt->name.line};
    emit_bytes(chunk, OpCode::DEFINE_GLOBAL, line, name_index, line);
  }

  template <typename T>
  auto operator()(T const& /*unused*/) -> void {}

  Chunk& chunk;
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
