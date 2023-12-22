#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include <string>
#include <vector>

#include "./types/object.h"
#include "./types/statement.h"
#include "environment.h"

struct Interpreter {
 public:
  auto interpret(std::vector<Statement const> const& statements) -> void;

  Environment<std::string, Object> environment_;
};

#endif
