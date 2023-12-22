#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include <vector>

#include "./environment.h"
#include "./types/statement.h"

struct Interpreter {
 public:
  auto interpret(std::vector<Statement const> const& statements) -> void;

  Environment environment_;
};

#endif
