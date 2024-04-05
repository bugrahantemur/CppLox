#include "./VM.hpp"

#include <functional>
#include <iostream>
#include <vector>

#include "../Common.hpp"
#include "../Compiler/Compiler.hpp"
#include "../Debug/Debug.hpp"
#include "../OpCode/OpCode.hpp"

namespace LOX::ByteCode::VM {

using LOX::Common::Types::Value;

VirtualMachine::VirtualMachine() {}
VirtualMachine::~VirtualMachine() {}

auto VirtualMachine::handle_op_constant(Value const& constant) -> void {
  stack.push(constant);
}

auto VirtualMachine::handle_op_negate() -> void {
  // Value const value{-stack.top()};
  stack.pop();
  // stack.push(value);
  stack.push({});
}

auto VirtualMachine::handle_op_return() -> void {
  Value const value{stack.top()};
  stack.pop();
  // std::cout << value << '\n';
}

auto VirtualMachine::run(Chunk const& chunk) -> InterpretResult {
  while (ip != chunk.code.end()) {
#ifdef DEBUG_TRACE_EXECUTION
    std::cout << "          ";
    auto debug_stack = stack;
    std::vector<Value> debug_vector;
    while (!debug_stack.empty()) {
      debug_vector.push_back(debug_stack.top());
      debug_stack.pop();
    }
    for (Value const& value : debug_vector) {
      std::cout << "[ " << value << " ]\n";
    }
    disassemble_instruction(chunk, std::distance(chunk.code.cbegin(), ip));
#endif

    Byte const instruction{*ip++};

    switch (instruction) {
      case OpCode::OP_CONSTANT:
        handle_op_constant(chunk.constants[*ip++]);
        break;
      case OpCode::OP_NEGATE:
        handle_op_negate();
        break;
      case OpCode::OP_ADD:
        handle_binary_op(std::plus<>{});
        break;
      case OpCode::OP_SUBTRACT:
        handle_binary_op(std::minus<>{});
        break;
      case OpCode::OP_MULTIPLY:
        handle_binary_op(std::multiplies<>{});
        break;
      case OpCode::OP_DIVIDE:
        handle_binary_op(std::divides<>{});
        break;
      case OpCode::OP_RETURN:
        handle_op_return();
        break;
      default:
        return InterpretResult::OK;
    }
  }
  return InterpretResult::OK;
}

auto VirtualMachine::interpret(std::string const& source) -> InterpretResult {
  Compiler::compile(source);

  return InterpretResult::OK;
}

}  // namespace LOX::ByteCode::VM
