#ifndef LOX_INTERPRETER_EXPRESSIONS_CALL
#define LOX_INTERPRETER_EXPRESSIONS_CALL

#include <exception>
#include <unordered_map>
#include <vector>

#include "../../../Types/Objects/Builtins/FunctionInterface.hpp"
#include "../../../Types/Objects/Class.hpp"
#include "../../../Types/Objects/Function.hpp"
#include "../../../Types/Objects/Object.hpp"
#include "../../../Types/Token/Token.hpp"
#include "../../../Utils/Arcdyn.hpp"
#include "../../../Utils/Box.hpp"
#include "../../Utils/Return/Return.hpp"

namespace LOX::Interpreter::Expressions {

using namespace Types::Objects;

struct UncallableError : public std::exception {};

auto call(Arc<Environment> environment,
          std::unordered_map<Token, std::size_t> const& resolution,
          std::vector<Object> const& args, Object const& callee) -> Object;

auto arity_of(Object const& callee) -> std::size_t;

}  // namespace LOX::Interpreter::Expressions

#endif
