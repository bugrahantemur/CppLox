#ifndef LOX_INTERPRETER_ERROR
#define LOX_INTERPRETER_ERROR

#include <string>

#include "../../Utils/Error.hpp"

namespace LOX::Interpreter {
class Error : public ErrorInterface {
 public:
  Error(std::size_t line, std::string const& message);
};
}  // namespace LOX::Interpreter

#endif