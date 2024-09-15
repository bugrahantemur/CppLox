#include <cstdlib>
#include <exception>

#include "../Common/Error/Interface/Interface.hpp"
#include "../Common/Error/Report/Report.hpp"
#include "../Common/Reader/Reader.hpp"
#include "./Chunk/Chunk.hpp"
#include "./Compiler/Compiler.hpp"
#include "./VM/VM.hpp"

namespace LOX::ByteCode {

static auto run_file(std::string const& path) -> void {
  std::string const source = Common::Reader::read(path);
  Chunk const chunk{Compiler::compile(source)};
  VM::interpret(chunk);
}

}  // namespace LOX::ByteCode

auto main(int argc, char* argv[]) -> int {
  if (argc != 2) {
    LOX::Common::Error::report(
        std::runtime_error{"Wrong usage! Correct usage: lox [script]\n"});
  }

  try {
    LOX::ByteCode::run_file(argv[1]);
  } catch (LOX::Common::Error::Interface const& e) {
    LOX::Common::Error::report(e);
    return EXIT_FAILURE;
  } catch (std::exception const& e) {
    LOX::Common::Error::report("Unhandled exception: ", e);
    return EXIT_FAILURE;
  } catch (...) {
    LOX::Common::Error::report(std::runtime_error{"Unknown error occured."});
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
