#ifndef LOX_SCANNER_ERROR
#define LOX_SCANNER_ERROR

#include <exception>
#include <string>

#include "../utils/error.hpp"

namespace LOX::Lexer {

class Error : public ErrorInterface {
 public:
  Error(std::size_t const line, std::string const& message);
};

auto error(std::size_t const line, std::string message) -> void;

}  // namespace LOX::Lexer

#endif
