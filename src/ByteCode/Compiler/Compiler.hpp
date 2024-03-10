#ifndef LOX_BYTECODE_COMPILER
#define LOX_BYTECODE_COMPILER

#include <string>

namespace LOX::ByteCode::Compiler {

auto compile(std::string const& source) -> void;

}

#endif
