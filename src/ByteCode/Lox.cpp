#include <cstdlib>
#include <iostream>

#include "../Common/Reader/Reader.hpp"
#include "./Chunk/Chunk.hpp"
#include "./Common.hpp"
#include "./Debug/Debug.hpp"
#include "./OpCode/OpCode.hpp"
#include "./VM/VM.hpp"

namespace LOX::ByteCode {

static void run_file(std::string const& path) {
  std::string const source = Common::Reader::read(path);

  VM::VirtualMachine vm{};

  VM::InterpretResult result = vm.interpret(source);

  if (result == VM::InterpretResult::COMPILE_ERROR) exit(65);
  if (result == VM::InterpretResult::RUNTIME_ERROR) exit(70);
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
