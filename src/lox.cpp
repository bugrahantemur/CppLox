#include <iostream>
#include <string>

#include "./interpreter/interpreter.hpp"
#include "./lexer/error.hpp"
#include "./lexer/lexer.hpp"
#include "./parser/error.hpp"
#include "./parser/parser.hpp"
#include "./resolver/error.hpp"
#include "./resolver/resolver.hpp"
#include "./types/function.hpp"
#include "./types/statement.hpp"
#include "./types/token.hpp"
#include "./utils/error.hpp"
#include "./utils/reader.hpp"

namespace LOX {
auto run(std::string const &file_path) -> void {
  try {
    std::string const contents = Reader::read_file(file_path);
    std::vector<Token> const tokens = Lexer::scan_tokens(contents);
    std::vector<Statement> const statements = Parser::parse(tokens);
    std::unordered_map<Token, std::size_t> const resolution =
        Resolver::resolve(statements);
    Interpreter::interpret(statements, resolution);
  } catch (std::exception const &e) {
    std::cerr << "Unhandled exception: " << e.what() << '\n';
    std::exit(-1);
    return;
  }
}
}  // namespace LOX

auto main(int argc, char *argv[]) -> int {
  if (argc != 2) {
    std::cout << "Wrong! Correct usage: cpplox [script]\n";
  } else {
    LOX::run(argv[1]);
  }

  return 0;
}
