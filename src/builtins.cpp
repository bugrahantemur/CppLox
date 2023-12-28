#include "./builtins.hpp"

#include <chrono>

consteval auto Clock::arity() -> std::size_t { return 0; }

consteval auto Clock::to_string() const -> std::string { return "Clock"; }

auto Clock::operator()(Interpreter& interpreter, std::vector<Object>& args)
    -> Object {
  static_cast<void>(interpreter);
  static_cast<void>(args);

  return static_cast<double>(
      std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count());
}

auto builtins() -> std::map<std::string, Object> {
  // TODO
  return {};
}

auto global_env() -> Environment {
  // TODO
  return {};
}
