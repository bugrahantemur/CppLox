#include <cstdlib>
#include <iostream>
#include <string>

#include "scanner.h"
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
      std::exit(EXIT_FAILURE);
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
  template <typename OStream>
  auto error(OStream &out, LoxError const &e) -> void {
    out << "[line " << e.line() << "] Error: " << e.what() << '\n';

    had_error = true;
  };

  auto run(std::string const &contents) -> void {
    try {
      std::vector<Token const> const tokens = Scanner::scan_tokens(contents);
      // do something with these tokens
      (void)tokens;
    } catch (LoxError const &e) {
      error(std::cerr, e);
    }
  }

  bool had_error{false};
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
  return EXIT_SUCCESS;
}
