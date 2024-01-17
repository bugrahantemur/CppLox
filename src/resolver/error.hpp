#ifndef LOX_RESOLVER_ERROR
#define LOX_RESOLVER_ERROR

#include <string>

namespace Resolver {
struct Error {
  Error(std::size_t line, std::string const& message);

  auto report() const -> void;

  std::size_t line_;
  std::string message_;
};
}  // namespace Resolver

#endif
