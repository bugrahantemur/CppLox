#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "../Common/Error/Interface/Interface.hpp"
#include "../Common/Error/Report/Report.hpp"
#include "../Common/Reader/Reader.hpp"
#include "../Common/Scanner/Scanner.hpp"
#include "../Common/Types/Tokens/Token.hpp"
#include "./Interpreter/Interpreter.hpp"
#include "./Parser/Parser.hpp"
#include "./Resolver/Resolver.hpp"
#include "./Types/Objects/Object.hpp"
#include "./Types/Syntax/Statement.hpp"

namespace LOX::TreeWalk {

auto run(std::string const &file_path) -> void {
  std::string const contents{LOX::Common::Reader::read(file_path)};

  std::vector<LOX::Common::Types::Token> const tokens{
      LOX::Common::Scanner::scan(contents)};

  std::vector<Types::Syntax::Statements::Statement> const statements{
      Parser::parse(tokens)};

  std::unordered_map<LOX::Common::Types::Token, std::size_t> const resolution{
      Resolver::resolve(statements)};

  Interpreter::interpret(statements, resolution);
}

}  // namespace LOX::TreeWalk

auto main(int argc, char *argv[]) -> int {
  if (argc != 2) {
    LOX::Common::Error::report(
        std::runtime_error{"Wrong usage! Correct usage: lox [script]\n"});
  } else {
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
  }

  return EXIT_SUCCESS;
}
