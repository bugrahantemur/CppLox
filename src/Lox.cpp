#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "./Error/Interface/Interface.hpp"
#include "./Error/Report/Report.hpp"
#include "./Interpreter/Interpreter.hpp"
#include "./Parser/Parser.hpp"
#include "./Reader/Reader.hpp"
#include "./Resolver/Resolver.hpp"
#include "./Scanner/Scanner.hpp"
#include "./Types/Objects/Object.hpp"
#include "./Types/Syntax/Statement.hpp"
#include "./Types/Tokens/Token.hpp"

namespace LOX {

auto run(std::string const &file_path) -> void {
  std::string const contents{Reader::read(file_path)};

  std::vector<Types::Tokens::Token> const tokens{Scanner::scan(contents)};

  std::vector<Types::Syntax::Statements::Statement> const statements{
      Parser::parse(tokens)};

  std::unordered_map<Types::Tokens::Token, std::size_t> const resolution{
      Resolver::resolve(statements)};

  Interpreter::interpret(statements, resolution);
}

}  // namespace LOX

auto main(int argc, char *argv[]) -> int {
  if (argc != 2) {
    LOX::Error::report(
        std::runtime_error{"Wrong usage! Correct usage: lox [script]\n"});
  } else {
    try {
      LOX::run(argv[1]);
    } catch (LOX::Error::Interface const &e) {
      LOX::Error::report(e);
      return EXIT_FAILURE;
    } catch (std::exception const &e) {
      LOX::Error::report("Unhandled exception: ", e);
      return EXIT_FAILURE;
    } catch (...) {
      LOX::Error::report(std::runtime_error{"Unknown error occured."});
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
