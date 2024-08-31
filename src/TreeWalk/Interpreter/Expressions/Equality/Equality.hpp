#ifndef LOX_TREEWALK_INTERPRETER_EXPRESSIONS_EQUALITY
#define LOX_TREEWALK_INTERPRETER_EXPRESSIONS_EQUALITY

#include <exception>

#include "../../../../Common/Types/Objects/Object.hpp"

namespace LOX::TreeWalk::Interpreter::Expressions {

struct NotComparableError : public std::exception {};

auto is_equal(Common::Types::Object const& left,
              Common::Types::Object const& right) -> bool;

}  // namespace LOX::TreeWalk::Interpreter::Expressions

#endif
