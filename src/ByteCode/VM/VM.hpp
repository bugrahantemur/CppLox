#ifndef LOX_BYTECODE_VM
#define LOX_BYTECODE_VM

#include "../Chunk/Chunk.hpp"

namespace LOX::ByteCode::VM {

auto interpret(Chunk const& chunk) -> void;

}  // namespace LOX::ByteCode::VM

#endif
