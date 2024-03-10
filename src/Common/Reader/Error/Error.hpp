#ifndef LOX_READER_ERROR
#define LOX_READER_ERROR

#include <string>

#include "../../../Common/Error/Interface/Interface.hpp"

namespace LOX::Common::Reader {

class Error : public LOX::Common::Error::Interface {
 public:
  Error(std::string const &message);
};

}  // namespace LOX::Common::Reader

#endif
