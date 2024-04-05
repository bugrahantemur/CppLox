#ifndef LOX_BYTECODE_CHUNK
#define LOX_BYTECODE_CHUNK

#include <vector>

#include "../../Common/Types/Value/Value.hpp"
#include "../Common.hpp"

namespace LOX::ByteCode {
struct Chunk {
  std::vector<Byte> code{};
  std::vector<Common::Types::Value> constants{};
  std::vector<std::size_t> lines{};

  auto append_byte(Byte byte, std::size_t line) -> std::size_t;
  auto append_constant(Common::Types::Value const& value) -> std::size_t;
};

}  // namespace LOX::ByteCode
#endif
