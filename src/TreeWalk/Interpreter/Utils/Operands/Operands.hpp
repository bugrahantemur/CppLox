#ifndef LOX_TREEWALK_INTERPRETER_UTILS_OPERANDS
#define LOX_TREEWALK_INTERPRETER_UTILS_OPERANDS

#include <algorithm>
#include <array>
#include <variant>

namespace LOX::TreeWalk::Interpreter::Utils::Operands {

template <typename... Objs>
auto check_number_operand(std::size_t line, Objs... operands) -> void {
  std::array const ops{operands...};
  if (std::all_of(std::begin(ops), std::end(ops), [](auto const& obj) {
        return !std::holds_alternative<double>(obj);
      })) {
    throw Error{line, sizeof...(operands) > 1 ? "Operands must be numbers."
                                              : "Operand must be a number."};
  }
}

}  // namespace LOX::TreeWalk::Interpreter::Utils::Operands

#endif
