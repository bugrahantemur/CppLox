
#ifndef LOX_INTERPRETER
#define LOX_INTERPRETER

#include <vector>

#include "statement.h"

namespace Interpreter {
auto interpret(std::vector<Statement const> const& statements) -> void;
}
#endif
