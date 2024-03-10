#ifndef LOX_BYTECODE_OPCODE
#define LOX_BYTECODE_OPCODE

#include <string>

namespace LOX::ByteCode {
enum OpCode : unsigned char {
  OP_CONSTANT,
  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NEGATE,
  OP_RETURN,
};
}

#endif
