#include "globals.hpp"

#include <chrono>

consteval auto Clock::arity() -> std::size_t { return 0; }

auto Clock::call(Interpreter& interpreter, std::vector<Object>& args)
    -> Object {
  static_cast<void>(interpreter);
  static_cast<void>(args);

  return static_cast<double>(
      std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count());
}

auto Clock::to_string() const -> std::string { return "Clock"; }
