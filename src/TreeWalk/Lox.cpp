#include <cstdlib>
#include <exception>
#include <string>
#include <vector>

#include "../Common/Error/Interface/Interface.hpp"
#include "../Common/Error/Report/Report.hpp"
#include "../Common/Parser/Parser.hpp"
#include "../Common/Reader/Reader.hpp"
#include "../Common/Resolver/Resolver.hpp"
#include "../Common/Scanner/Scanner.hpp"
#include "../Common/Types/Syntax/Statement.hpp"
#include "../Common/Types/Tokens/Token.hpp"
#include "./Interpreter/Interpreter.hpp"

namespace LOX::TreeWalk {

auto run(std::string const &file_path) -> void {
  std::string const contents{Common::Reader::read(file_path)};

  std::vector<Common::Types::Token> const tokens{
      LOX::Common::Scanner::scan(contents)};

  std::vector<Common::Types::Syntax::Statements::Statement> const statements{
      Common::Parser::parse(tokens)};

  auto const [resolution, depth]{Common::Resolver::resolve(statements)};
  // depth is not needed for the treewalk interpreter
  (void)depth;

  Interpreter::interpret(statements, resolution);
}

}  // namespace LOX::TreeWalk

auto main(int argc, char *argv[]) -> int {
  if (argc != 2) {
    LOX::Common::Error::report(
        std::runtime_error{"Wrong usage! Correct usage: lox [script]\n"});
    return EXIT_FAILURE;
  }

  try {
    LOX::TreeWalk::run(argv[1]);
  } catch (LOX::Common::Error::Interface const &e) {
    LOX::Common::Error::report(e);
    return EXIT_FAILURE;
  } catch (std::exception const &e) {
    LOX::Common::Error::report("Unhandled exception: ", e);
    return EXIT_FAILURE;
  } catch (...) {
    LOX::Common::Error::report(std::runtime_error{"Unknown error occured."});
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
