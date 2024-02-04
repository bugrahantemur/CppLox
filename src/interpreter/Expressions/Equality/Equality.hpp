#ifndef LOX_INTERPRETER_EXPRESSIONS_EQUALITY
#define LOX_INTERPRETER_EXPRESSIONS_EQUALITY

#include <exception>

#include "../../../Types/Object/Object.hpp"

namespace LOX::Interpreter::Expressions {

struct NotComparableError : public std::exception {};

auto is_equal(Types::Objects::Object const& left,
              Types::Objects::Object const& right) -> bool;

}  // namespace LOX::Interpreter::Expressions

#endif
