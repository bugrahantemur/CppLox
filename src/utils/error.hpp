#ifndef LOX_UTILS_ERROR
#define LOX_UTILS_ERROR

#include <exception>
#include <iostream>

namespace LOX {

class ErrorInterface : public std::runtime_error {
 public:
  ErrorInterface(std::string const& msg) : std::runtime_error(msg) {}
};

inline auto report(std::string const& custom_msg, std::exception const& e)
    -> void {
  std::cerr << custom_msg << e.what() << '\n';
}

inline auto report(std::exception const& e) -> void { report("", e); }

}  // namespace LOX
#endif
