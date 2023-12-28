#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include <string>
#include <vector>

#include "./environment.hpp"
#include "./types/object.hpp"
#include "./types/statement.hpp"

struct Interpreter {
 public:
  Interpreter();

  explicit Interpreter(Environment const& environment);

  auto interpret(std::vector<Statement> const& statements) -> void;

  Environment environment_;
};

#endif
