#include "./VM.hpp"

#include <functional>
#include <stack>
#include <unordered_map>
#include <vector>

#include "../../Common/Types/Objects/Object.hpp"
#include "../../Common/Types/Value/Value.hpp"
#include "../../Common/Utils/Operands/Operands.hpp"
#include "../../Common/Utils/Put/Put.hpp"
#include "../../Common/Utils/Truth/Truth.hpp"
#include "../OpCode/OpCode.hpp"
#include "../Types/Byte.hpp"
#include "./Error/Error.hpp"

namespace LOX::ByteCode::VM {

using LOX::Common::Types::Object;
using LOX::Common::Types::Value;

class VirtualMachine {
 public:
  explicit VirtualMachine(Chunk const& chunk) : chunk{chunk} {}

  auto run() -> void {
    while (ip < chunk.code.size()) {
      Byte const instruction{chunk.code[ip++]};

      switch (instruction) {
        case OpCode::CONSTANT:
          handle_op_constant();
          break;

        case OpCode::NIL:
          handle_op_simple(std::monostate{});
          break;

        case OpCode::TRUE:
          handle_op_simple(true);
          break;

        case OpCode::FALSE:
          handle_op_simple(false);
          break;

        case OpCode::POP:
          handle_op_pop();
          break;

        case OpCode::DEFINE_GLOBAL:
          handle_op_define_global();
          break;

        case OpCode::NEGATE:
          handle_op_negate();
          break;

        case OpCode::ADD:
          handle_op_binary(number_or_string_op(std::plus{}));
          break;

        case OpCode::SUBTRACT:
          handle_op_binary(number_op(std::minus{}));
          break;

        case OpCode::MULTIPLY:
          handle_op_binary(number_op(std::multiplies{}));
          break;

        case OpCode::DIVIDE:
          handle_op_binary(number_op(std::divides{}));
          break;

        case OpCode::NOT:
          handle_op_not();
          break;

        case OpCode::RETURN:
          handle_op_return();
          break;

        case OpCode::EQUAL:
          handle_op_binary(number_or_string_op(std::equal_to{}));
          break;

        case OpCode::NOT_EQUAL:
          handle_op_binary(number_or_string_op(std::not_equal_to{}));
          break;

        case OpCode::LESS:
          handle_op_binary(number_or_string_op(std::less{}));
          break;

        case OpCode::LESS_EQUAL:
          handle_op_binary(number_or_string_op(std::less_equal{}));
          break;

        case OpCode::GREATER:
          handle_op_binary(number_or_string_op(std::greater{}));
          break;

        case OpCode::GREATER_EQUAL:
          handle_op_binary(number_or_string_op(std::greater_equal{}));
          break;

        case OpCode::PRINT:
          handle_op_print();
          break;

        default:
          break;
      }
    }
  }

 private:
  auto handle_op_define_global() -> void {
    std::size_t const global_index{chunk.code[ip++]};
    auto const name{std::get<std::string>(chunk.constants[global_index])};

    globals.insert_or_assign(name, stack.top());
    stack.pop();
  };

  auto handle_op_print() -> void {
    Object const object{stack.top()};
    stack.pop();

    std::visit(Common::Utils::Put{std::cout}, object);
  }

  auto handle_op_pop() -> void { stack.pop(); }

  template <typename T>
  auto handle_op_simple(T const& object) -> void {
    stack.push(object);
  }

  auto handle_op_constant() -> void {
    std::size_t const constant_idx{chunk.code[ip++]};
    stack.emplace(
        Common::Types::Objects::to_object(chunk.constants[constant_idx]));
  }

  auto handle_op_not() -> void {
    Object const object{stack.top()};
    stack.pop();

    stack.emplace(!Common::Utils::is_truthy(object));
  }

  auto handle_op_negate() -> void {
    Object const object{stack.top()};
    Common::Utils::Operands::check_number_operand<Error>(chunk.lines[ip - 1],
                                                         object);
    stack.pop();
    stack.emplace(-std::get<double>(object));
  }

  auto handle_op_return() -> void {
    Object const object{stack.top()};
    stack.pop();
  }

  template <typename Op>
  auto handle_op_binary(Op op) -> void {
    Common::Types::Object const right = stack.top();
    stack.pop();

    Common::Types::Object const left = stack.top();
    stack.pop();

    stack.push(op(left, right));
  }

  template <typename Op>
  auto number_or_string_op(Op const& op)
      -> std::function<Object(Object const&, Object const&)> {
    return [this, op](Object const& left, Object const& right) -> Object {
      // Numbers
      if (std::holds_alternative<double>(left) &&
          std::holds_alternative<double>(right)) {
        return op(std::get<double>(left), std::get<double>(right));
      }
      // Strings
      if (std::holds_alternative<std::string>(left) &&
          std::holds_alternative<std::string>(right)) {
        return op(std::get<std::string>(left), std::get<std::string>(right));
      }
      throw Error{chunk.lines[ip],
                  "Operands must be two numbers or two strings."};
    };
  }

  template <typename Op>
  auto number_op(Op const& op)
      -> std::function<Object(Object const&, Object const&)> {
    return [this, op](Object const& left, Object const& right) -> Object {
      Common::Utils::Operands::check_number_operand<Error>(chunk.lines[ip],
                                                           left, right);

      return op(std::get<double>(left), std::get<double>(right));
    };
  }

  std::size_t ip{0};
  std::stack<Common::Types::Object> stack;
  std::unordered_map<std::string, Common::Types::Object> globals;

  Chunk const& chunk;
};

auto interpret(Chunk const& chunk) -> void {
  VirtualMachine vm{chunk};
  vm.run();
}
}  // namespace LOX::ByteCode::VM
