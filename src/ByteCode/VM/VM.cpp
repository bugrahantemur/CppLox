#include "./VM.hpp"

#include <stack>
#include <vector>

#include "../../Common/Utils/Operands/Operands.hpp"
#include "../../Common/Utils/Truth/Truth.hpp"
#include "../Common.hpp"
#include "../OpCode/OpCode.hpp"
#include "./Error/Error.hpp"

namespace LOX::ByteCode::VM {

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

        case OpCode::NEGATE:
          handle_op_negate();
          break;

        case OpCode::ADD:
          handle_binary_op([this](Value const& left,
                                  Value const& right) -> Value {
            // Numbers
            if (std::holds_alternative<double>(left) &&
                std::holds_alternative<double>(right)) {
              return std::get<double>(left) + std::get<double>(right);
            }
            // Strings
            if (std::holds_alternative<std::string>(left) &&
                std::holds_alternative<std::string>(right)) {
              return std::get<std::string>(left) + std::get<std::string>(right);
            }
            throw Error{chunk.lines[ip],
                        "Operands must be two numbers or two strings."};
          });
          break;

        case OpCode::SUBTRACT:
          handle_binary_op(
              [this](Value const& left, Value const& right) -> Value {
                Common::Utils::Operands::check_number_operand<Error>(
                    chunk.lines[ip], left, right);
                return std::get<double>(left) - std::get<double>(right);
              });
          break;

        case OpCode::MULTIPLY:
          handle_binary_op(
              [this](Value const& left, Value const& right) -> Value {
                Common::Utils::Operands::check_number_operand<Error>(
                    chunk.lines[ip], left, right);
                return std::get<double>(left) * std::get<double>(right);
              });
          break;

        case OpCode::DIVIDE:
          handle_binary_op(
              [this](Value const& left, Value const& right) -> Value {
                Common::Utils::Operands::check_number_operand<Error>(
                    chunk.lines[ip], left, right);
                return std::get<double>(left) / std::get<double>(right);
              });
          break;

        case OpCode::NOT:
          handle_op_not();
          break;

        case OpCode::RETURN:
          handle_op_return();
          break;

        case OpCode::EQUAL:
          handle_binary_op(
              [this](Value const& left, Value const& right) -> Value {
                // Numbers
                if (std::holds_alternative<double>(left) &&
                    std::holds_alternative<double>(right)) {
                  return std::get<double>(left) == std::get<double>(right);
                }
                // Strings
                if (std::holds_alternative<std::string>(left) &&
                    std::holds_alternative<std::string>(right)) {
                  return std::get<std::string>(left) ==
                         std::get<std::string>(right);
                }
                throw Error{chunk.lines[ip],
                            "Operands must be two numbers or two strings."};
              });
          break;

        case OpCode::NOT_EQUAL:
          handle_binary_op(
              [this](Value const& left, Value const& right) -> Value {
                // Numbers
                if (std::holds_alternative<double>(left) &&
                    std::holds_alternative<double>(right)) {
                  return std::get<double>(left) != std::get<double>(right);
                }
                // Strings
                if (std::holds_alternative<std::string>(left) &&
                    std::holds_alternative<std::string>(right)) {
                  return std::get<std::string>(left) !=
                         std::get<std::string>(right);
                }
                throw Error{chunk.lines[ip],
                            "Operands must be two numbers or two strings."};
              });
          break;

        case OpCode::LESS:
          handle_binary_op([this](Value const& left,
                                  Value const& right) -> Value {
            // Numbers
            if (std::holds_alternative<double>(left) &&
                std::holds_alternative<double>(right)) {
              return std::get<double>(left) < std::get<double>(right);
            }
            // Strings
            if (std::holds_alternative<std::string>(left) &&
                std::holds_alternative<std::string>(right)) {
              return std::get<std::string>(left) < std::get<std::string>(right);
            }
            throw Error{chunk.lines[ip],
                        "Operands must be two numbers or two strings."};
          });
          break;

        case OpCode::LESS_EQUAL:
          handle_binary_op(
              [this](Value const& left, Value const& right) -> Value {
                // Numbers
                if (std::holds_alternative<double>(left) &&
                    std::holds_alternative<double>(right)) {
                  return std::get<double>(left) <= std::get<double>(right);
                }
                // Strings
                if (std::holds_alternative<std::string>(left) &&
                    std::holds_alternative<std::string>(right)) {
                  return std::get<std::string>(left) <=
                         std::get<std::string>(right);
                }
                throw Error{chunk.lines[ip],
                            "Operands must be two numbers or two strings."};
              });
          break;

        case OpCode::GREATER:
          handle_binary_op([this](Value const& left,
                                  Value const& right) -> Value {
            // Numbers
            if (std::holds_alternative<double>(left) &&
                std::holds_alternative<double>(right)) {
              return std::get<double>(left) > std::get<double>(right);
            }
            // Strings
            if (std::holds_alternative<std::string>(left) &&
                std::holds_alternative<std::string>(right)) {
              return std::get<std::string>(left) > std::get<std::string>(right);
            }
            throw Error{chunk.lines[ip],
                        "Operands must be two numbers or two strings."};
          });
          break;

        case OpCode::GREATER_EQUAL:
          handle_binary_op(
              [this](Value const& left, Value const& right) -> Value {
                // Numbers
                if (std::holds_alternative<double>(left) &&
                    std::holds_alternative<double>(right)) {
                  return std::get<double>(left) >= std::get<double>(right);
                }
                // Strings
                if (std::holds_alternative<std::string>(left) &&
                    std::holds_alternative<std::string>(right)) {
                  return std::get<std::string>(left) >=
                         std::get<std::string>(right);
                }
                throw Error{chunk.lines[ip],
                            "Operands must be two numbers or two strings."};
              });
          break;

        default:
          break;
      }
    }
  }

 private:
  template <typename T>
  auto handle_op_simple(T const& value) -> void {
    stack.push(value);
  }

  auto handle_op_constant() -> void {
    std::size_t const constant_idx{chunk.code[ip++]};
    stack.push(chunk.constants[constant_idx]);
  }

  auto handle_op_not() -> void {
    Value const value{stack.top()};
    stack.pop();

    stack.emplace(!Common::Utils::is_truthy(value));
  }

  auto handle_op_negate() -> void {
    Value const value{stack.top()};
    Common::Utils::Operands::check_number_operand<Error>(chunk.lines[ip - 1],
                                                         value);
    stack.pop();
    stack.emplace(-std::get<double>(value));
  }

  auto handle_op_return() -> void {
    Value const value{stack.top()};
    stack.pop();
  }

  template <typename Op>
  auto handle_binary_op(Op const& op) -> void {
    Common::Types::Value const right = stack.top();
    stack.pop();

    Common::Types::Value const left = stack.top();
    stack.pop();

    stack.push(op(left, right));
  }

  std::size_t ip{0};
  std::stack<Common::Types::Value> stack;

  Chunk const& chunk;
};

auto interpret(Chunk const& chunk) -> void {
  VirtualMachine vm{chunk};
  vm.run();
}
}  // namespace LOX::ByteCode::VM
