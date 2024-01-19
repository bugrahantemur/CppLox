#ifndef LOX_RESOLVER_ERROR
#define LOX_RESOLVER_ERROR

#include <string>

#include "../utils/error_interface.hpp"

namespace LOX::Resolver {

class Error : ErrorInterface {
 public:
  Error(std::size_t line, std::string const& message);

  auto report() const -> void final;

 private:
  std::size_t line_;
  std::string message_;
};

}  // namespace LOX::Resolver

#endif
