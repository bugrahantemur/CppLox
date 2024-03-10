#include "./Report.hpp"

#include <exception>
#include <iostream>
#include <string>

namespace LOX::Common::Error {

auto report(std::string const& custom_msg, std::exception const& e) -> void {
  std::cerr << custom_msg << e.what() << '\n';
}

auto report(std::exception const& e) -> void { report("", e); }

}  // namespace LOX::Common::Error
