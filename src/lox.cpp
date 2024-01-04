#include <iostream>
#include <string>

#include "./interpreter.hpp"
#include "./parser/parser.hpp"
#include "./resolver.hpp"
#include "./scanner.hpp"
#include "./types/expression.hpp"
#include "./types/function.hpp"
#include "./types/statement.hpp"
#include "./types/token.hpp"
#include "./utils/error.hpp"
#include "./utils/reader.hpp"

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
    Expression expr;
    try {
      std::vector<Token> const tokens = Scanner::scan_tokens(contents);
      std::vector<Statement> const statements = Parser::parse(tokens);
      std::map<Token, std::size_t> const resolution =
          Resolver::resolve(statements);
      interpreter.interpret(statements, resolution);
    } catch (CompileTimeError const &e) {
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

  Interpreter interpreter;

  bool had_error{false};
  bool had_runtime_error{false};
};

auto main(int argc, char *argv[]) -> int {
  if (argc != 2) {
    std::cout << "Wrong! Correct usage: cpplox [script]\n";
  } else {
    Lox lox;
    lox.run(argv[1]);
  }
  return 0;
}
