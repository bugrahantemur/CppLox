#ifndef LOX_INTERPRETER_EXPRESSION_EVALUATE
#define LOX_INTERPRETER_EXPRESSION_EVALUATE

#include <unordered_map>

#include "../../../Common/Types/Tokens/Token.hpp"
#include "../../Types/Objects/Object.hpp"
#include "../../Types/Syntax/Statement.hpp"

namespace LOX::TreeWalk::Interpreter::Expressions {

auto evaluate(Types::Syntax::Expressions::Expression const& expression,
              Arc<Types::Environment> environment,
              std::unordered_map<LOX::Common::Types::Token, std::size_t> const&
                  resolution) -> Types::Objects::Object;

}

#endif
