#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include <vector>

#include "./types/object.h"
#include "./types/statement.h"
#include "environment.h"

struct Interpreter {
 public:
  auto interpret(std::vector<Statement const> const& statements) -> void;

  Environment<Object> environment_;
};

#endif
