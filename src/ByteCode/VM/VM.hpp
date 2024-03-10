#ifndef LOX_BYTECODE_VM
#define LOX_BYTECODE_VM

#include <memory>
#include <stack>

#include "../Chunk/Chunk.hpp"
#include "../Value/Value.hpp"

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
  auto handle_op_constant(Chunk const& chunk) -> void;
  auto handle_op_add(Chunk const& chunk) -> void;
  auto handle_op_subtract(Chunk const& chunk) -> void;
  auto handle_op_multiply(Chunk const& chunk) -> void;
  auto handle_op_divide(Chunk const& chunk) -> void;
  auto handle_op_negate(Chunk const& chunk) -> void;
  auto handle_op_return(Chunk const& chunk) -> void;

  IP ip;
  std::stack<Value> stack;
};

}  // namespace LOX::ByteCode::VM

#endif
