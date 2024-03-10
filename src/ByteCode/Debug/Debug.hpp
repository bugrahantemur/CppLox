#ifndef LOX_BYTECODE_DEBUG
#define LOX_BYTECODE_DEBUG

#include <string>

#include "../Chunk/Chunk.hpp"

namespace LOX::ByteCode {
auto disassemble_chunk(Chunk const& chunk, std::string const& name) -> void;
auto disassemble_instruction(Chunk const& chunk, std::size_t idx)
    -> std::size_t;
}  // namespace LOX::ByteCode
#endif
