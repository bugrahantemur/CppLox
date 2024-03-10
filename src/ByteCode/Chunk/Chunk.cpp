#include "./Chunk.hpp"

#include <cassert>
#include <vector>

#include "../OpCode/OpCode.hpp"
#include "../Value/Value.hpp"

namespace LOX::ByteCode {

auto Chunk::add_instruction(Byte byte, std::size_t line) -> std::size_t {
  code.push_back(byte);
  lines.push_back(line);

  assert(lines.size() == code.size());

  return code.size() - 1;
}

auto Chunk::add_constant(Value const& constant) -> std::size_t {
  constants.push_back(constant);
  return constants.size() - 1;
}

}  // namespace LOX::ByteCode
