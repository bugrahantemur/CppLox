#ifndef LOX_INTERPRETER_ERROR
#define LOX_INTERPRETER_ERROR

#include <string>

#include "../../../Common/Error/Interface/Interface.hpp"

namespace LOX::TreeWalk::Interpreter {
class Error : public LOX::Common::Error::Interface {
 public:
  Error(std::size_t line, std::string const& message);
};
}  // namespace LOX::TreeWalk::Interpreter

#endif
