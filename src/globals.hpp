#ifndef LOX_GLOBALS
#define LOX_GLOBALS

#include <time.h>

#include <vector>

#include "./interpreter.hpp"
#include "./types/object.hpp"

struct Clock {
  consteval auto arity() -> std::size_t;

  auto call(Interpreter& interpreter, std::vector<Object>& args) -> Object;

  auto to_string() const -> std::string;
};

#endif
