#ifndef LOX_UTILS_ERROR
#define LOX_UTILS_ERROR

#include <iostream>
#include <string>

inline auto report(std::size_t const line, std::string const& where,
                   std::string const& message) -> void {
  std::cerr << "[line " << line << "] Error: " << where << ": " << message
            << '\n';
};

#endif
