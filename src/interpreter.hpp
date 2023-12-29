#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include <memory>
#include <string>
#include <vector>

#include "./environment.hpp"
#include "./types/object.hpp"
#include "./types/statement.hpp"

struct Interpreter {
 public:
  Interpreter();

  explicit Interpreter(std::shared_ptr<Environment> const& environment);

  auto interpret(std::vector<Statement> const& statements) -> void;

  std::shared_ptr<Environment> environment_;
};

#endif
