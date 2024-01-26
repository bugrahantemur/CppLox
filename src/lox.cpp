#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "./interpreter/interpreter.hpp"
#include "./lexer/lexer.hpp"
#include "./objects/object.hpp"
#include "./parser/parser.hpp"
#include "./resolver/resolver.hpp"
#include "./syntax/statement.hpp"
#include "./syntax/token.hpp"
#include "./utils/error.hpp"
#include "./utils/reader.hpp"

namespace LOX {

auto run(std::string const &file_path) -> void {
  std::string const contents = Reader::read(file_path);

  std::vector<Token> const tokens = Lexer::scan(contents);

  std::vector<Statement> const statements = Parser::parse(tokens);

  std::unordered_map<Token, std::size_t> const resolution =
      Resolver::resolve(statements);

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
