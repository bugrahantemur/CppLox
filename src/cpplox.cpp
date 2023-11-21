#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

auto print_prompt_marker(std::ostream &out) -> void { out << "> "; }

auto print_too_many_arguments(std::ostream &out) -> void {
  out << "Too many arguments.\nUsage: cpplox [script]\n";
}

template <typename IStream, typename F>
auto for_each_line(IStream &in, F const &f) {
  for (std::string line; std::getline(in, line);) {
    f(line);
  }
}

auto read_file(std::string const &path) -> std::string {
  std::ifstream file(path);
  std::string contents;

  for_each_line(file,
                [&contents](auto const &line) { contents += line + '\n'; });

  return contents;
}

struct Token {
  std::string content;
};

template <typename OStream>
OStream &operator<<(OStream &out, Token const &token) {
  out << "--> " << token.content << '\n';

  return out;
}

auto scan_tokens(std::string const &contents) -> std::vector<Token> {
  return {Token{contents}};
}

class Lox {
 public:
  template <typename OStream>
  auto error(OStream &out, int const line, std::string const &message) -> void {
    report(out, line, "", message);
  };

  auto run_file(std::string const &file_path) -> void {
    auto const file_contents = read_file(file_path);

    run(file_contents);
  }

  auto run_prompt() -> void {
    print_prompt_marker(std::cout);

    for_each_line(std::cin, [this](auto const &line) {
      run(line);
      print_prompt_marker(std::cout);
    });
  };

 private:
  template <typename OStream>
  auto report(OStream &out, int const line, std::string const &where,
              std::string const &message) -> void {
    out << "[line " << line << "] Error" << where << ": " << message;

    had_error = true;
  }

  auto run(std::string const &contents) -> void {
    std::vector<Token> const tokens = scan_tokens(contents);

    for (auto const &token : tokens) {
      std::cout << token;
    }
  }

  bool had_error{false};
};

auto main(int argc, char *argv[]) -> int {
  if (argc > 2) {
    print_too_many_arguments(std::cout);
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
