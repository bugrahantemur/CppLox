#include <iostream>
#include <string>

#include "expression.h"
#include "interpreter.h"
#include "parser.h"
#include "scanner.h"
#include "statement.h"
#include "token.h"
#include "utils/error.h"
#include "utils/printer.h"
#include "utils/reader.h"

class Lox {
 public:
  auto run_file(std::string const &file_path) -> void {
    auto const file_contents = Reader::read_file(file_path);

    run(file_contents);

    if (had_error) {
      std::exit(65);
    }

    if (had_runtime_error) {
      std::exit(70);
    }
  }

  auto run_prompt() -> void {
    Printer::print_prompt_marker(std::cout);

    for (std::string line; std::getline(std::cin, line);) {
      run(line);
      had_error = false;
      Printer::print_prompt_marker(std::cout);
    }
  };

 private:
  auto run(std::string const &contents) -> void {
    Expression expr;
    try {
      std::vector<Token const> const tokens = Scanner::scan_tokens(contents);
      std::vector<Statement const> const statements = Parser::parse(tokens);
      Interpreter::interpret(statements);
    } catch (RuntimeError const &e) {
      had_runtime_error = true;
      e.report();
      return;
    } catch (LoxError const &e) {
      had_error = true;
      e.report();
      return;
    }
  }

  bool had_error{false};
  bool had_runtime_error{false};
};

auto main(int argc, char *argv[]) -> int {
  if (argc > 2) {
    Printer::print_too_many_arguments(std::cout);
  } else {
    Lox lox;
    if (argc == 2) {
      lox.run_file(argv[1]);
    } else {
      lox.run_prompt();
    }
  }
  return 0;
}
