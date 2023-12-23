#ifndef LOX_GLOBALS
#define LOX_GLOBALS

#include <time.h>

#include <string_view>
#include <vector>

#include "./environment.hpp"
#include "./interpreter.hpp"
#include "./types/object.hpp"

struct Clock {
  consteval auto arity() -> std::size_t;

  consteval auto to_string() const -> std::string_view;

  auto operator()(Interpreter& interpreter, std::vector<Object>& args)
      -> Object;
};

auto builtins() -> std::map<std::string, Object>;

#endif
