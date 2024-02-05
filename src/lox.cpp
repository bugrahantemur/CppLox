#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "./Interpreter/Interpreter.hpp"
#include "./Parser/Parser.hpp"
#include "./Resolver/Resolver.hpp"
#include "./Scanner/Scanner.hpp"
#include "./Types/Objects/Object.hpp"
#include "./Types/Syntax/Statement.hpp"
#include "./Types/Token/Token.hpp"
#include "./Utils/Error.hpp"
#include "./Utils/Reader.hpp"

namespace LOX {

auto run(std::string const &file_path) -> void {
  std::string const contents{Reader::read(file_path)};

  std::vector<Token> const tokens{Scanner::scan(contents)};

  std::vector<Types::Syntax::Statements::Statement> const statements{
      Parser::parse(tokens)};

  std::unordered_map<Token, std::size_t> const resolution{
      Resolver::resolve(statements)};

  Interpreter::interpret(statements, resolution);
}

}  // namespace LOX

auto main(int argc, char *argv[]) -> int {
  if (argc != 2) {
    LOX::report(
        std::runtime_error{"Wrong usage! Correct usage: cpplox [script]\n"});
  } else {
    try {
      LOX::run(argv[1]);
    } catch (LOX::ErrorInterface const &e) {
      LOX::report(e);
      return EXIT_FAILURE;
    } catch (std::exception const &e) {
      LOX::report("Unhandled exception: ", e);
      return EXIT_FAILURE;
    } catch (...) {
      LOX::report(std::runtime_error{"Unknown error occured."});
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
