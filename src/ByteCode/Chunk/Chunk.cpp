#include "./Chunk.hpp"

#include <cassert>

#include "../../Common/Types/Value/Value.hpp"

namespace LOX::ByteCode {

auto Chunk::append_byte(Byte byte, std::size_t line) -> std::size_t {
  code.push_back(byte);
  lines.push_back(line);

  assert(lines.size() == code.size());

  return code.size() - 1;
}

auto Chunk::append_constant(Common::Types::Value const& constant)
    -> std::size_t {
  constants.push_back(constant);
  return constants.size() - 1;
}

}  // namespace LOX::ByteCode
