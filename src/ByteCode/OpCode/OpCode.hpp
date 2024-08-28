#ifndef LOX_BYTECODE_OPCODE
#define LOX_BYTECODE_OPCODE

namespace LOX::ByteCode::OpCode {

enum OpCode : unsigned char {
  CONSTANT,
  NIL,
  TRUE,
  FALSE,
  EQUAL,
  NOT_EQUAL,
  LESS,
  LESS_EQUAL,
  GREATER,
  GREATER_EQUAL,
  ADD,
  SUBTRACT,
  MULTIPLY,
  DIVIDE,
  NOT,
  NEGATE,
  RETURN,
};

}  // namespace LOX::ByteCode::OpCode

#endif
