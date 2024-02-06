#ifndef LOX_INTERPRETER_EXPRESSION_EVALUATE
#define LOX_INTERPRETER_EXPRESSION_EVALUATE

#include <unordered_map>

#include "../../Types/Objects/Object.hpp"
#include "../../Types/Syntax/Statement.hpp"
#include "../../Types/Token/Token.hpp"

namespace LOX::Interpreter::Expressions {

auto evaluate(Types::Syntax::Expressions::Expression const& expression,
              Arc<Types::Environment> environment,
              std::unordered_map<Token, std::size_t> const& resolution)
    -> Types::Objects::Object;

}

#endif
