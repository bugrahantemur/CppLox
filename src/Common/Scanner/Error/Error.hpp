#ifndef LOX_SCANNER_ERROR
#define LOX_SCANNER_ERROR

#include <exception>
#include <string>

#include "../../Error/Interface/Interface.hpp"

namespace LOX::Common::Scanner {

class Error : public LOX::Common::Error::Interface {
 public:
  Error(std::size_t const line, std::string const& message);
};

auto error(std::size_t const line, std::string message) -> void;

}  // namespace LOX::Common::Scanner

#endif
