#include <string>

class LoxError : public std::exception {
 public:
  LoxError(int line, std::string const& message)
      : line_(line), message_(message) {}

  [[nodiscard]] auto what() const noexcept -> char const* {
    return message_.c_str();
  }

  [[nodiscard]] auto line() const noexcept -> int { return line_; }

 private:
  int line_;
  std::string message_;
};
