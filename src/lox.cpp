#include <iostream>
#include <string>

#include "./interpreter.hpp"
#include "./lexer/error.hpp"
#include "./lexer/lexer.hpp"
#include "./parser/error.hpp"
#include "./parser/parser.hpp"
#include "./resolver.hpp"
#include "./types/expression.hpp"
#include "./types/function.hpp"
#include "./types/statement.hpp"
#include "./types/token.hpp"
#include "./utils/error.hpp"
#include "./utils/reader.hpp"

namespace LOX {
class Lox {
 public:
  auto run(std::string const &file_path) -> void {
    auto const file_contents = Reader::read_file(file_path);

    do_run(file_contents);

    if (had_error) {
      std::exit(65);
    }

    if (had_runtime_error) {
      std::exit(70);
    }
  }

 private:
  auto do_run(std::string const &contents) -> void {
    try {
      std::vector<Token> const tokens = Lexer::scan_tokens(contents);
      std::vector<Statement> const statements = Parser::parse(tokens);
      std::unordered_map<Token, std::size_t> const resolution =
          Resolver::resolve(statements);
      Interpreter::interpret(statements, resolution);
    } catch (Lexer::Error const &e) {
      had_error = true;
      e.report();
      return;
    } catch (Parser::Error const &e) {
      had_error = true;
      e.report();
      return;
    } catch (RuntimeError const &e) {
      had_runtime_error = true;
      e.report();
      return;
    } catch (std::exception const &e) {
      std::cerr << "Unhandled exception: " << e.what() << '\n';
      std::exit(-1);
      return;
    }
  }

  bool had_error{false};
  bool had_runtime_error{false};
};

}  // namespace LOX
auto main(int argc, char *argv[]) -> int {
  if (argc != 2) {
    std::cout << "Wrong! Correct usage: cpplox [script]\n";
  } else {
    LOX::Lox lox;
    lox.run(argv[1]);
  }
  return 0;
}
