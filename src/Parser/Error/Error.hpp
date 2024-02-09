#ifndef LOX_PARSER_ERROR
#define LOX_PARSER_ERROR

#include <exception>
#include <string>
#include <vector>

#include "../../Error/Interface/Interface.hpp"
#include "../../Types/Tokens/Token.hpp"

namespace LOX::Parser {
class Error : public LOX::Error::Interface {
 public:
  Error(std::size_t const line, std::string const& where,
        std::string const& message);
};

auto error(Types::Tokens::Token const& token, std::string const& message)
    -> Error;

}  // namespace LOX::Parser

#endif
