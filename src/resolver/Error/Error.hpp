#ifndef LOX_RESOLVER_ERROR
#define LOX_RESOLVER_ERROR

#include <string>

#include "../../Utils/error.hpp"

namespace LOX::Resolver {

class Error : public ErrorInterface {
 public:
  Error(std::size_t line, std::string const& message);
};

}  // namespace LOX::Resolver

#endif
