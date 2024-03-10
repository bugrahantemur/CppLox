#ifndef LOX_READER_ERROR
#define LOX_READER_ERROR

#include <string>

#include "../../../Common/Error/Interface/Interface.hpp"

namespace LOX::Reader {

class Error : public LOX::Error::Interface {
 public:
  Error(std::string const &message);
};

}  // namespace LOX::Reader

#endif
