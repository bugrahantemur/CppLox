#ifndef LOX_PARSER_ERROR
#define LOX_PARSER_ERROR

#include <exception>
#include <string>
#include <vector>

#include "../../../Common/Error/Interface/Interface.hpp"
#include "../../../Common/Types/Tokens/Token.hpp"

namespace LOX::TreeWalk::Parser {
class Error : public LOX::Common::Error::Interface {
 public:
  Error(std::size_t const line, std::string const& where,
        std::string const& message);
};

auto error(LOX::Common::Types::Token const& token, std::string const& message)
    -> Error;

}  // namespace LOX::TreeWalk::Parser

#endif
