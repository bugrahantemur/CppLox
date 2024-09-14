#ifndef LOX_BYTECODE_CHUNK
#define LOX_BYTECODE_CHUNK

#include <vector>

#include "../../Common/Types/Value/Value.hpp"
#include "../Types/Byte.hpp"

namespace LOX::ByteCode {

class Chunk {
 public:
  auto append_byte(Byte byte, std::size_t line) -> std::size_t;
  auto append_constant(Common::Types::Value const& value) -> std::size_t;

  [[nodiscard]] auto size() const -> std::size_t { return code.size(); }

  [[nodiscard]] auto byte_at(std::size_t pos) const -> Byte {
    return code.at(pos);
  }
  [[nodiscard]] auto constant_at(std::size_t pos) const
      -> Common::Types::Value {
    return constants.at(pos);
  }
  [[nodiscard]] auto line_at(std::size_t pos) const -> std::size_t {
    return lines.at(pos);
  }

 private:
  std::vector<Byte> code;
  std::vector<Common::Types::Value> constants;
  std::vector<std::size_t> lines;
};

}  // namespace LOX::ByteCode

#endif
