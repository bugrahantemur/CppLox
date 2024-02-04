#ifndef LOX_INTERPRETER_UTILS_OPERANDS
#define LOX_INTERPRETER_UTILS_OPERANDS

#include "../../../Types/Object/Object.hpp"
#include "../../../Types/Token/Token.hpp"

namespace LOX::Interpreter::Utils::Operands {

template <typename... Objs>
auto check_number_operand(Token const& token, Objs... operands) -> void {
  std::array const ops{operands...};
  if (std::all_of(std::begin(ops), std::end(ops),
                  [](Types::Objects::Object const& obj) {
                    return !std::holds_alternative<double>(obj);
                  })) {
    throw Error{token.line, sizeof...(operands) > 1
                                ? "Operands must be numbers."
                                : "Operand must be a number."};
  }
}

}  // namespace LOX::Interpreter::Utils::Operands
#endif