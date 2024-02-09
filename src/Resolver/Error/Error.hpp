#ifndef LOX_RESOLVER_ERROR
#define LOX_RESOLVER_ERROR

#include <string>

#include "../../Error/Interface/Interface.hpp"

namespace LOX::Resolver {

class Error : public LOX::Error::Interface {
 public:
  Error(std::size_t line, std::string const& message);
};

}  // namespace LOX::Resolver

#endif
