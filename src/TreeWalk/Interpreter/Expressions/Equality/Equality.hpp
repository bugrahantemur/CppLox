#ifndef LOX_INTERPRETER_EXPRESSIONS_EQUALITY
#define LOX_INTERPRETER_EXPRESSIONS_EQUALITY

#include <exception>

#include "../../../Types/Objects/Object.hpp"

namespace LOX::TreeWalk::Interpreter::Expressions {

struct NotComparableError : public std::exception {};

auto is_equal(Types::Objects::Object const& left,
              Types::Objects::Object const& right) -> bool;

}  // namespace LOX::TreeWalk::Interpreter::Expressions

#endif
