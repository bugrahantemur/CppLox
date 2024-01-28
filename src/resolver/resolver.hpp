#include <unordered_map>
#include <vector>

#include "../syntax_types/statement.hpp"

namespace LOX::Resolver {

auto resolve(std::vector<Statement> const& statements)
    -> std::unordered_map<Token, std::size_t>;

}  // namespace LOX::Resolver
