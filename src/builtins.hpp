#ifndef LOX_GLOBALS
#define LOX_GLOBALS

#include <time.h>

#include <map>
#include <string>
#include <vector>

// #include "./interpreter.hpp"
// #include "./types/object.hpp"

struct Clock {
  consteval auto arity() -> std::size_t;

  consteval auto to_string() const -> std::string;

  // auto operator()(Interpreter& interpreter, std::vector<Object>& args)
  //     -> Object;
};

auto builtins() -> std::map<std::string, int>;

#endif
