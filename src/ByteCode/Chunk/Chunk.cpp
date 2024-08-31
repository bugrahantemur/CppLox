#include "./Chunk.hpp"

#include <cassert>

#include "../../Common/Types/Objects/Object.hpp"

namespace LOX::ByteCode {

auto Chunk::append_byte(Byte byte, std::size_t line) -> std::size_t {
  code.push_back(byte);
  lines.push_back(line);

  assert(lines.size() == code.size());

  return code.size() - 1;
}

auto Chunk::append_object(Common::Types::Object const& object) -> std::size_t {
  objects.emplace_back(object);
  return objects.size() - 1;
}

auto Chunk::append_constant(Common::Types::Value const& value) -> std::size_t {
  objects.emplace_back(
      std::visit([](auto&& val) { return Common::Types::Object{val}; }, value));
  return objects.size() - 1;
}

}  // namespace LOX::ByteCode
