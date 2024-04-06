#include <cstdlib>
#include <iostream>

#include "../Common/Reader/Reader.hpp"
#include "./Chunk/Chunk.hpp"
#include "./Compiler/Compiler.hpp"
#include "./VM/VM.hpp"

namespace LOX::ByteCode {

static auto run_file(std::string const& path) -> void {
  try {
    std::string const source = Common::Reader::read(path);
    Chunk const chunk{Compiler::compile(source)};
    VM::interpret(chunk);
  } catch (...) {
    std::exit(1);
  }
}

}  // namespace LOX::ByteCode

auto main(int argc, char* argv[]) -> int {
  if (argc == 2) {
    LOX::ByteCode::run_file(argv[1]);
  } else {
    std::cerr << "Usage: clox [path]\n";
    exit(64);
  }

  return EXIT_SUCCESS;
}
