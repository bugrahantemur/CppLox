#include "./VM.hpp"

#include <iostream>
#include <vector>

#include "../Common.hpp"
#include "../Compiler/Compiler.hpp"
#include "../Debug/Debug.hpp"
#include "../OpCode/OpCode.hpp"

namespace LOX::ByteCode::VM {

VirtualMachine::VirtualMachine() {}
VirtualMachine::~VirtualMachine() {}

inline auto VirtualMachine::handle_op_constant(Chunk const& chunk) -> void {
  Value const constant = chunk.constants[*ip++];
  stack.push(constant);
}

inline auto VirtualMachine::handle_op_negate(Chunk const& chunk) -> void {
  Value const value{-stack.top()};
  stack.pop();
  stack.push(value);
}

inline auto VirtualMachine::handle_op_return(Chunk const& chunk) -> void {
  Value const value{stack.top()};
  stack.pop();
  std::cout << value << '\n';
}

inline auto binary_op(std::stack<Value>& stack,
                      std::function<Value(Value, Value)> const& op) -> void {
  double const b = stack.top();
  stack.pop();
  double const a = stack.top();
  stack.pop();
  stack.push(op(a, b));
}

inline auto VirtualMachine::handle_op_add(Chunk const& chunk) -> void {
  binary_op(stack, [](auto const& a, auto const& b) { return a + b; });
}
inline auto VirtualMachine::handle_op_subtract(Chunk const& chunk) -> void {
  binary_op(stack, [](auto const& a, auto const& b) { return a * b; });
}
inline auto VirtualMachine::handle_op_multiply(Chunk const& chunk) -> void {
  binary_op(stack, [](auto const& a, auto const& b) { return a / b; });
}
inline auto VirtualMachine::handle_op_divide(Chunk const& chunk) -> void {
  binary_op(stack, [](auto const& a, auto const& b) { return a / b; });
}

#undef BINARY_OP

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
        handle_op_constant(chunk);
        break;
      case OpCode::OP_NEGATE:
        handle_op_negate(chunk);
        break;
      case OpCode::OP_ADD:
        handle_op_add(chunk);
        break;
      case OpCode::OP_SUBTRACT:
        handle_op_subtract(chunk);
        break;
      case OpCode::OP_MULTIPLY:
        handle_op_multiply(chunk);
        break;
      case OpCode::OP_DIVIDE:
        handle_op_divide(chunk);
        break;
      case OpCode::OP_RETURN:
        handle_op_return(chunk);
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
