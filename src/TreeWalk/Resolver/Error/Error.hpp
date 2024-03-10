#ifndef LOX_RESOLVER_ERROR
#define LOX_RESOLVER_ERROR

#include <string>

#include "../../../Common/Error/Interface/Interface.hpp"

namespace LOX::TreeWalk::Resolver {

class Error : public LOX::Common::Error::Interface {
 public:
  Error(std::size_t line, std::string const& message);
};

}  // namespace LOX::TreeWalk::Resolver

#endif
