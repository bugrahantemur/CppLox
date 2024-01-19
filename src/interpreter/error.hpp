#ifndef LOX_INTERPRETER_ERROR
#define LOX_INTERPRETER_ERROR

#include <string>

#include "../utils/error_interface.hpp"

namespace LOX::Interpreter {
class Error : ErrorInterface {
 public:
  Error(std::size_t line, std::string const& message);

  auto report() const -> void override;

 private:
  std::size_t line_;
  std::string message_;
};
}  // namespace LOX::Interpreter

#endif
