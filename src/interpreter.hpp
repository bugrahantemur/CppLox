#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include <string>
#include <vector>

#include "./types/object.hpp"
#include "./types/statement.hpp"
#include "environment.hpp"

struct Interpreter {
 public:
  Interpreter() = default;

  explicit Interpreter(Environment<std::string, Object> const& environment)
      : environment_{environment} {}

  auto interpret(std::vector<Statement const> const& statements) -> void;

  Environment<std::string, Object> environment_;
};

#endif
