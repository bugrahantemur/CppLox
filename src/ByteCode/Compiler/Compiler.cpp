#include "./Compiler.hpp"

#include <limits>
#include <stdexcept>
#include <variant>

#include "../../Common/Parser/Parser.hpp"
#include "../../Common/Scanner/Scanner.hpp"
#include "../../Common/Types/Syntax/Expression.hpp"
#include "../../Common/Types/Syntax/Statement.hpp"
#include "../../Common/Types/Tokens/Token.hpp"
#include "../Chunk/Chunk.hpp"
#include "../OpCode/OpCode.hpp"

namespace LOX::ByteCode::Compiler {

using LOX::Common::Types::Token;
using LOX::Common::Types::Syntax::Statements::Statement;
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

auto expression(Chunk& chunk, Expression const& expr) -> void {}

auto unary(Chunk& chunk, UnaryExpr const& expr) -> void {
  // Compile the operand
  expression(chunk, expr.right);

  // Emit the operator instruction
  switch (expr.op.type) {
    case Common::Types::Tokens::TokenType::MINUS:
      emit_byte(chunk, OpCode::OP_NEGATE, expr.op.line);
      break;
    default:
      return;  // Unreachable
  }
}

auto number(Chunk& chunk, Value const& value) -> void {
  emit_constant(chunk, value);
}

auto compile(std::string const& source) -> void {
  std::vector<Token> tokens = Common::Scanner::scan(source);
  std::vector<Statement> statements = Common::Parser::parse(tokens);

  Chunk chunk;

  for (auto const& stmt : statements) {
    // Pass
  }
}

}  // namespace LOX::ByteCode::Compiler
