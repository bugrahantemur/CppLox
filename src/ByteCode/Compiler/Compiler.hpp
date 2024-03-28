#ifndef LOX_BYTECODE_COMPILER
#define LOX_BYTECODE_COMPILER

#include <string>

#include "../Chunk/Chunk.hpp"

namespace LOX::ByteCode::Compiler {

auto compile(std::string const& source) -> Chunk;

}

#endif
