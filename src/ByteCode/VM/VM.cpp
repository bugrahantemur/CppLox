#include "./VM.hpp"

#include <functional>
#include <stack>
#include <vector>

#include "../../Common/Utils/Operands/Operands.hpp"
#include "../Common.hpp"
#include "../OpCode/OpCode.hpp"
#include "./Error/Error.hpp"

namespace LOX::ByteCode::VM {

using LOX::Common::Types::Value;

class VirtualMachine {
 public:
  VirtualMachine(Chunk const& chunk) : ip{0}, stack{}, chunk{chunk} {}

  auto run() -> void {
    while (ip < chunk.code.size()) {
      Byte const instruction{chunk.code[ip++]};

      if (instruction == OpCode::OP_CONSTANT) {
        handle_op_constant();
      } else if (instruction == OpCode::OP_NEGATE) {
        handle_op_negate();
      } else if (instruction == OpCode::OP_ADD) {
        handle_binary_op(std::plus<>{});
      } else if (instruction == OpCode::OP_SUBTRACT) {
        handle_binary_op(std::minus<>{});
      } else if (instruction == OpCode::OP_MULTIPLY) {
        handle_binary_op(std::multiplies<>{});
      } else if (instruction == OpCode::OP_DIVIDE) {
        handle_binary_op(std::divides<>{});
      } else if (instruction == OpCode::OP_RETURN) {
        handle_op_return();
      } else {
        //
      }
    }
  }

 private:
  auto handle_op_constant() -> void {
    std::size_t const constant_idx{chunk.code[ip++]};
    stack.push(chunk.constants[constant_idx]);
  }

  auto handle_op_negate() -> void {
    Value const value{stack.top()};
    stack.pop();

    Common::Utils::Operands::check_number_operand<Error>(chunk.lines[ip],
                                                         value);
    stack.push(-std::get<double>(value));
  }

  auto handle_op_return() -> void {
    Value const value{stack.top()};
    stack.pop();
  }

  template <typename Op>
  auto handle_binary_op(Op op) -> void {
    Common::Types::Value const b = stack.top();
    stack.pop();

    Common::Types::Value const a = stack.top();
    stack.pop();

    // stack.push(op(a, b));
  }

  std::size_t ip;
  std::stack<Common::Types::Value> stack;

  Chunk const& chunk;
};

auto interpret(Chunk const& chunk) -> void {
  VirtualMachine vm{chunk};
  vm.run();
}
}  // namespace LOX::ByteCode::VM
