#ifndef LOX_UTILS_ERROR
#define LOX_UTILS_ERROR

#include <iostream>
#include <stdexcept>
#include <string>

#include "../token.h"

struct RuntimeError : public std::runtime_error {
  RuntimeError(Token const& token, std::string const& message)
      : token_(token), std::runtime_error(message) {}

  Token token_;
};

inline auto report(RuntimeError const& e) -> void {
  std::cerr << "[line " << e.token_.line_ << "] " << e.what() << '\n';
}

inline auto report(std::size_t const line, std::string const& where,
                   std::string const& message) -> void {
  std::cerr << "[line " << line << "] Error: " << where << ": " << message
            << '\n';
};

#endif
