#ifndef LOX_COMMON_ERROR_REPORT
#define LOX_COMMON_ERROR_REPORT

#include <exception>
#include <string>

namespace LOX::Common::Error {

auto report(std::string const& custom_msg, std::exception const& e) -> void;

auto report(std::exception const& e) -> void;

}  // namespace LOX::Common::Error

#endif
