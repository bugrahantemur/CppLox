#include "./Debug.hpp"

#include <iomanip>
#include <iostream>
#include <string>

#include "../Chunk/Chunk.hpp"
#include "../OpCode/OpCode.hpp"

namespace LOX::ByteCode {

auto to_string(OpCode const& byte) -> std::string {
  switch (byte) {
    case OpCode::OP_CONSTANT:
      return "OP_CONSTANT";
    case OpCode::OP_RETURN:
      return "OP_RETURN";
    default:
      return "";
  }
}

auto disassemble_chunk(Chunk& chunk, std::string const& name) -> void {
  std::cout << "== " << name << " ==\n";

  for (int idx_of_instruction{0}; idx_of_instruction < chunk.code.size();) {
    idx_of_instruction = disassemble_instruction(chunk, idx_of_instruction);
  }
}

auto simple_instruction(std::string const& name, std::size_t idx_of_instruction)
    -> std::size_t {
  std::cout << name << '\n';
  return idx_of_instruction + 1;
}

auto constant_instruction(std::string const& name, Chunk const& chunk,
                          std::size_t idx_of_instruction) -> std::size_t {
  auto const idx_of_constant =
      static_cast<std::size_t>(chunk.code[idx_of_instruction + 1]);
  std::cout << name << ' ' << std::setw(4) << std::setfill('0')
            << idx_of_constant << " '" << chunk.constants[idx_of_constant]
            << "'\n";

  return idx_of_instruction + 2;
}

auto disassemble_instruction(Chunk const& chunk, std::size_t idx_of_instruction)
    -> std::size_t {
  std::cout << std::setw(5) << std::setfill('0') << idx_of_instruction << ' ';

  if (idx_of_instruction > 0 &&
      chunk.lines[idx_of_instruction] == chunk.lines[idx_of_instruction - 1]) {
    std::cout << "   | ";
  } else {
    std::cout << std::setw(4) << std::setfill(' ')
              << chunk.lines[idx_of_instruction] << ' ';
  }

  OpCode const instruction{chunk.code[idx_of_instruction]};

  switch (instruction) {
    case OpCode::OP_CONSTANT:
      return constant_instruction("OP_CONSTANT", chunk, idx_of_instruction);
    case OpCode::OP_ADD:
      return simple_instruction("OP_ADD", idx_of_instruction);
    case OpCode::OP_SUBTRACT:
      return simple_instruction("OP_SUBTRACT", idx_of_instruction);
    case OpCode::OP_MULTIPLY:
      return simple_instruction("OP_MULTIPLY", idx_of_instruction);
    case OpCode::OP_DIVIDE:
      return simple_instruction("OP_DIVIDE", idx_of_instruction);
    case OpCode::OP_NEGATE:
      return simple_instruction("OP_NEGATE", idx_of_instruction);
    case OpCode::OP_RETURN:
      return simple_instruction("OP_RETURN", idx_of_instruction);
    default:
      std::cout << "Unknown opcode " << to_string(instruction) << '\n';
      return idx_of_instruction + 1;
  }
}
}  // namespace LOX::ByteCode
