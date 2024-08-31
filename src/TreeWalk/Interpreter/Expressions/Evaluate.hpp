#ifndef LOX_TREEWALK_INTERPRETER_EXPRESSION_EVALUATE
#define LOX_TREEWALK_INTERPRETER_EXPRESSION_EVALUATE

#include <unordered_map>

#include "../../../Common/Types/Objects/Object.hpp"
#include "../../../Common/Types/Tokens/Token.hpp"

namespace LOX::TreeWalk::Interpreter::Expressions {

auto evaluate(Common::Types::Syntax::Expressions::Expression const& expression,
              Arc<Types::Environment> environment,
              std::unordered_map<LOX::Common::Types::Token, std::size_t> const&
                  resolution) -> Common::Types::Object;

}

#endif
