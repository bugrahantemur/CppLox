#ifndef LOX_TREEWALK_RESOLVER_ERROR
#define LOX_TREEWALK_RESOLVER_ERROR

#include <string>

#include "../../../Common/Error/Interface/Interface.hpp"

namespace LOX::Common::Resolver {

class Error : public LOX::Common::Error::Interface {
 public:
  Error(std::size_t line, std::string const& message);
};

}  // namespace LOX::Common::Resolver

#endif
