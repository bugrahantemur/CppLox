#include <cstdlib>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include "./interpreter/interpreter.hpp"
#include "./lexer/lexer.hpp"
#include "./parser/parser.hpp"
#include "./resolver/resolver.hpp"
#include "./types/object.hpp"
#include "./types/statement.hpp"
#include "./types/token.hpp"
#include "./utils/error_interface.hpp"
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
    std::cout << "Wrong usage! Correct usage: cpplox [script]\n";
  } else {
    try {
      LOX::run(argv[1]);
    } catch (LOX::ErrorInterface const &e) {
      e.report();
      return EXIT_FAILURE;
    } catch (std::exception const &e) {
      std::cerr << "Unhandled exception: " << e.what() << '\n';
      return EXIT_FAILURE;
    }
  }

  return EXIT_SUCCESS;
}
