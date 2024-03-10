#ifndef LOX_ERROR_REPORT
#define LOX_ERROR_REPORT

#include <exception>
#include <string>

namespace LOX::Error {

auto report(std::string const& custom_msg, std::exception const& e) -> void;

auto report(std::exception const& e) -> void;

}  // namespace LOX::Error

#endif
