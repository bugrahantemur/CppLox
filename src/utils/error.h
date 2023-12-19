#ifndef LOX_UTILS_ERROR
#define LOX_UTILS_ERROR

#include <iostream>
#include <stdexcept>
#include <string>

#include "../token.h"

struct SyntaxError {
  std::size_t line_;
  std::string where_;
  std::string message_;

  auto report() const -> void {
    std::cerr << "[line " << line_ << "] Error: " << where_ << ": " << message_
              << '\n';
  }
};

struct RuntimeError {
  std::size_t line_;
  std::string message_;

  auto report() const -> void {
    std::cerr << "[line " << line_ << "] " << message_ << '\n';
  }
};

inline auto report(std::size_t const line, std::string const& where,
                   std::string const& message) -> void {
  std::cerr << "[line " << line << "] Error: " << where << ": " << message
            << '\n';
};

#endif
