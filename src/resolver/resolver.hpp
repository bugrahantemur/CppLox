#include <iostream>
#include <unordered_map>
#include <vector>

#include "../interpreter/interpreter.hpp"
#include "../types/statement.hpp"
#include "../utils/error.hpp"

namespace Resolver {
auto resolve(std::vector<Statement> const& statements)
    -> std::unordered_map<Token, std::size_t>;
}  // namespace Resolver
