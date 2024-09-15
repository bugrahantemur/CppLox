#ifndef LOX_TREEWALK_RESOLVER
#define LOX_TREEWALK_RESOLVER

#include <array>
#include <unordered_map>
#include <vector>

#include "../../Common/Types/Syntax/Statement.hpp"

namespace LOX::Common::Resolver {

auto resolve(
    std::vector<Common::Types::Syntax::Statements::Statement> const& statements)
    -> std::array<std::unordered_map<LOX::Common::Types::Token, std::size_t>,
                  2>;

}  // namespace LOX::Common::Resolver

#endif
