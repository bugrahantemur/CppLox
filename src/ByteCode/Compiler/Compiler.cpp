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
#include "../Chunk/Chunk.hpp"
#include "../OpCode/OpCode.hpp"

namespace LOX::ByteCode::Compiler {

using LOX::Common::Types::Token;
using LOX::Common::Types::Syntax::Statements::Statement;
using LOX::Common::Types::Tokens::TokenType;
using namespace LOX::Common::Types::Syntax::Expressions;

auto emit_byte(Chunk& chunk, Byte byte, std::size_t line) -> void {
  chunk.add_instruction(byte, line);
}

auto emit_bytes(Chunk& chunk, Byte byte1, std::size_t line1, Byte byte2,
                std::size_t line2) -> void {
  chunk.add_instruction(byte1, line1);
  chunk.add_instruction(byte2, line2);
}

auto error(std::string const& msg) { throw std::runtime_error(msg); }

auto make_constant(Chunk& chunk, Value const& value) -> Byte {
  std::size_t const_idx{chunk.add_constant(value)};
  if (const_idx > static_cast<std::size_t>(std::numeric_limits<Byte>::max())) {
    error("Too many constants in one chunk.");
    return 0;  // Unreachable
  }

  return static_cast<Byte>(const_idx);
}

auto emit_constant(Chunk& chunk, Value const& value) -> void {
  emit_bytes(chunk, OpCode::OP_CONSTANT, 0, make_constant(chunk, value), 0);
}

struct ExpressionCompiler {
  auto operator()(std::monostate) -> void {}

  auto operator()(Box<LiteralExpr> const& expr) -> void {}

  auto operator()(Box<SuperExpr> const& expr) -> void {}

  auto operator()(Box<ThisExpr> const& expr) -> void {}

  auto operator()(Box<VariableExpr> const& expr) -> void {}

  auto operator()(Box<AssignmentExpr> const& expr) -> void {}

  auto operator()(Box<BinaryExpr> const& expr) -> void {
    // Compile the operands
    std::visit(*this, expr->left);
    std::visit(*this, expr->right);

    // Emit the operator instruction
    Token const& op{expr->op};
    TokenType const op_type{op.type};
    std::size_t const op_line{op.line};

    if (op_type == TokenType::PLUS) {
      emit_byte(chunk, OpCode::OP_ADD, op_line);
    } else if (op_type == TokenType::MINUS) {
      emit_byte(chunk, OpCode::OP_SUBTRACT, op_line);
    } else if (op_type == TokenType::STAR) {
      emit_byte(chunk, OpCode::OP_MULTIPLY, op_line);
    } else if (op_type == TokenType::SLASH) {
      emit_byte(chunk, OpCode::OP_DIVIDE, op_line);
    }
  }

  auto operator()(Box<CallExpr> const& expr) -> void {}

  auto operator()(Box<GetExpr> const& expr) -> void {}

  auto operator()(Box<GroupingExpr> const& expr) -> void {
    std::visit(*this, expr->expression);
  }

  auto operator()(Box<LogicalExpr> const& expr) -> void {}

  auto operator()(Box<SetExpr> const& expr) -> void {}

  auto operator()(Box<UnaryExpr> const& expr) -> void {
    // Compile the operand
    std::visit(*this, expr->right);

    // Emit the operator instruction
    Token const& op{expr->op};
    TokenType const op_type{op.type};

    if (op_type == TokenType::MINUS) {
      emit_byte(chunk, OpCode::OP_NEGATE, expr->op.line);
    }

    if (op_type == TokenType::BANG) {
    }
  }

  Chunk& chunk;
};

struct StatementCompiler {
  template <typename T>
  auto operator()(T const&) -> void {}

  Chunk& chunk;
};

auto number(Chunk& chunk, Value const& value) -> void {
  emit_constant(chunk, value);
}

auto compile(std::string const& source) -> Chunk {
  std::vector<Token> tokens = Common::Scanner::scan(source);
  std::vector<Statement> statements = Common::Parser::parse(tokens);

  Chunk chunk;

  for (auto const& stmt : statements) {
    std::visit(StatementCompiler{chunk}, stmt);
  }

  return chunk;
}

}  // namespace LOX::ByteCode::Compiler
