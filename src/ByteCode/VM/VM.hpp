#ifndef LOX_BYTECODE_VM
#define LOX_BYTECODE_VM

#include <stack>

#include "../../Common/Types/Value/Value.hpp"
#include "../Chunk/Chunk.hpp"

namespace LOX::ByteCode::VM {

enum class InterpretResult {
  OK,
  COMPILE_ERROR,
  RUNTIME_ERROR,
};

class VirtualMachine {
 public:
  VirtualMachine();

  ~VirtualMachine();

  auto run(Chunk const& chunk) -> InterpretResult;

  auto interpret(std::string const& source) -> InterpretResult;

 private:
  auto handle_op_constant(Common::Types::Value const& constant) -> void;
  auto handle_op_negate() -> void;
  auto handle_op_return() -> void;

  template <typename Op>
  auto handle_binary_op(Op op) -> void {
    Common::Types::Value const b = stack.top();
    stack.pop();

    Common::Types::Value const a = stack.top();
    stack.pop();

    // stack.push(op(a, b));
  }

  IP ip;
  std::stack<Common::Types::Value> stack;
};

}  // namespace LOX::ByteCode::VM

#endif
