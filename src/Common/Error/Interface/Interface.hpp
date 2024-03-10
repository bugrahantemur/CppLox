#ifndef LOX_ERROR_ERROR
#define LOX_ERROR_ERROR

#include <exception>
#include <iostream>

namespace LOX::Common::Error {

class Interface : public std::runtime_error {
 public:
  Interface(std::string const& msg) : std::runtime_error(msg){};
};

}  // namespace LOX::Common::Error

#endif
