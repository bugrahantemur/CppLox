#ifndef LOX_RESOLVER
#define LOX_RESOLVER

#include <unordered_map>
#include <vector>

#include "../Types/Syntax/Statement.hpp"

namespace LOX::Resolver {

auto resolve(
    std::vector<Types::Syntax::Statements::Statement> const& statements)
    -> std::unordered_map<Token, std::size_t>;

}  // namespace LOX::Resolver

#endif
