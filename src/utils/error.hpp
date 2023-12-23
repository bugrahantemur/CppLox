#ifndef LOX_UTILS_ERROR
#define LOX_UTILS_ERROR

#include <iostream>
#include <stdexcept>
#include <string>

#include "../types/token.hpp"

struct LoxError : std::exception {
  virtual auto report() const -> void = 0;
};

struct RuntimeError : LoxError {
  RuntimeError(std::size_t line, std::string const& message)
      : line_(line), message_(message) {}

  std::size_t line_;
  std::string message_;

  auto report() const -> void final {
    std::cerr << "[line " << line_ << "] " << message_ << '\n';
  }
};

#endif
