#ifndef LOX_UTILS_ERROR
#define LOX_UTILS_ERROR

#include <string>

class LoxError : public std::exception {
 public:
  LoxError(std::size_t line, std::string const& message)
      : line_(line), message_(message) {}

  [[nodiscard]] auto what() const noexcept -> char const* final {
    return message_.c_str();
  }

  [[nodiscard]] auto line() const noexcept -> int { return line_; }

 private:
  std::size_t line_;
  std::string message_;
};

#endif
