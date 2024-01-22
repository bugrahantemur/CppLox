#ifndef LOX_PARSER_ERROR
#define LOX_PARSER_ERROR

#include <exception>
#include <string>
#include <vector>

#include "../types/token.hpp"
#include "../utils/error.hpp"

namespace LOX::Parser {
class Error : public ErrorInterface {
 public:
  Error(std::size_t const line, std::string const& where,
        std::string const& message);
};

auto error(Token const& token, std::string const& message) -> Error;

}  // namespace LOX::Parser

#endif
