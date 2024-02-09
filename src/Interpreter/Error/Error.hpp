#ifndef LOX_INTERPRETER_ERROR
#define LOX_INTERPRETER_ERROR

#include <string>

#include "../../Error/Interface/Interface.hpp"

namespace LOX::Interpreter {
class Error : public LOX::Error::Interface {
 public:
  Error(std::size_t line, std::string const& message);
};
}  // namespace LOX::Interpreter

#endif
