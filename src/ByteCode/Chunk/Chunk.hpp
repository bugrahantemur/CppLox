#ifndef LOX_BYTECODE_CHUNK
#define LOX_BYTECODE_CHUNK

#include <vector>

#include "../Common.hpp"
#include "../Value/Value.hpp"

namespace LOX::ByteCode {
struct Chunk {
  std::vector<Byte> code{};
  std::vector<Value> constants{};
  std::vector<std::size_t> lines{};

  auto add_instruction(Byte byte, std::size_t line) -> std::size_t;
  auto add_constant(Value const& value) -> std::size_t;
};

}  // namespace LOX::ByteCode
#endif
