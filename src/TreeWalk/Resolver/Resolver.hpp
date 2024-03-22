#ifndef LOX_TREEWALK_RESOLVER
#define LOX_TREEWALK_RESOLVER

#include <unordered_map>
#include <vector>

#include "../../Common/Types/Syntax/Statement.hpp"

namespace LOX::TreeWalk::Resolver {

auto resolve(
    std::vector<Common::Types::Syntax::Statements::Statement> const& statements)
    -> std::unordered_map<LOX::Common::Types::Token, std::size_t>;

}  // namespace LOX::TreeWalk::Resolver

#endif
