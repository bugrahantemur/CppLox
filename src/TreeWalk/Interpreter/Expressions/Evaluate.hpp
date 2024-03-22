#ifndef LOX_TREEWALK_INTERPRETER_EXPRESSION_EVALUATE
#define LOX_TREEWALK_INTERPRETER_EXPRESSION_EVALUATE

#include <unordered_map>

#include "../../../Common/Types/Syntax/Statement.hpp"
#include "../../../Common/Types/Tokens/Token.hpp"
#include "../../Types/Objects/Object.hpp"

namespace LOX::TreeWalk::Interpreter::Expressions {

auto evaluate(Common::Types::Syntax::Expressions::Expression const& expression,
              Arc<Types::Environment> environment,
              std::unordered_map<LOX::Common::Types::Token, std::size_t> const&
                  resolution) -> Types::Objects::Object;

}

#endif
