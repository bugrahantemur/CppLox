#include "./VM.hpp"

#include <cassert>
#include <cstddef>
#include <functional>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <variant>

#include "../../Common/Types/Objects/Object.hpp"
#include "../../Common/Types/Value/Value.hpp"
#include "../../Common/Utils/Operands/Operands.hpp"
#include "../../Common/Utils/Put/Put.hpp"
#include "../../Common/Utils/Truth/Truth.hpp"
#include "../Chunk/Chunk.hpp"
#include "../OpCode/OpCode.hpp"
#include "../Types/Byte.hpp"
#include "./Error/Error.hpp"

namespace LOX::ByteCode::VM {

using LOX::Common::Types::Object;
using LOX::Common::Types::Value;

class VirtualMachine {
 public:
  explicit VirtualMachine(Chunk const& chunk) : chunk_{&chunk} {}

  auto run() -> void {
    while (ip_ < chunk_->size()) {
      Byte const instruction{chunk_->byte_at(ip_++)};

      switch (instruction) {
        case OpCode::CONSTANT:
          handle_op_constant_at();
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

        case OpCode::GET_GLOBAL:
          handle_op_get_global();
          break;

        case OpCode::SET_GLOBAL:
          handle_op_set_global();
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
  auto handle_op_get_global() -> void {
    std::size_t const global_index{chunk_->byte_at(ip_++)};
    auto const name{std::get<std::string>(chunk_->constant_at(global_index))};

    if (auto const it{globals_.find(name)}; it != globals_.end()) {
      stack_.push(it->second);
    } else {
      throw Error{chunk_->line_at(ip_ - 1),
                  "Undefined variable '" + name + "'."};
    }
  };

  auto handle_op_define_global() -> void {
    std::size_t const global_index{chunk_->byte_at(ip_++)};
    auto const name{std::get<std::string>(chunk_->constant_at(global_index))};

    globals_.insert_or_assign(name, stack_.top());
    stack_.pop();
  };

  auto handle_op_set_global() -> void {
    std::size_t const global_index{chunk_->byte_at(ip_++)};
    auto const name{std::get<std::string>(chunk_->constant_at(global_index))};

    if (auto it{globals_.find(name)}; it != globals_.end()) {
      it->second = stack_.top();
      // popping here is not necessary (in contrast to handle_op_define_global),
      // since this comes from an expression statement, and therefore has a pop
      // at the end automatically
      // i.e. just replace the value and do nothing else
    } else {
      throw Error{chunk_->line_at(ip_ - 1),
                  "Undefined variable '" + name + "'."};
    }
  };

  auto handle_op_print() -> void {
    Object const object{stack_.top()};
    stack_.pop();

    std::visit(Common::Utils::Put{std::cout}, object);
  }

  auto handle_op_pop() -> void { stack_.pop(); }

  template <typename T>
  auto handle_op_simple(T const& object) -> void {
    stack_.push(object);
  }

  auto handle_op_constant_at() -> void {
    std::size_t const constant_idx{chunk_->byte_at(ip_++)};
    stack_.emplace(
        Common::Types::Objects::to_object(chunk_->constant_at(constant_idx)));
  }

  auto handle_op_not() -> void {
    Object const object{stack_.top()};
    stack_.pop();

    stack_.emplace(!Common::Utils::is_truthy(object));
  }

  auto handle_op_negate() -> void {
    Object const object{stack_.top()};
    Common::Utils::Operands::check_number_operand<Error>(
        chunk_->line_at(ip_ - 1), object);
    stack_.pop();
    stack_.emplace(-std::get<double>(object));
  }

  auto handle_op_return() -> void {
    Object const object{stack_.top()};
    stack_.pop();
  }

  template <typename Op>
  auto handle_op_binary(Op op) -> void {
    Common::Types::Object const right = stack_.top();
    stack_.pop();

    Common::Types::Object const left = stack_.top();
    stack_.pop();

    stack_.push(op(left, right));
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
      throw Error{chunk_->line_at(ip_),
                  "Operands must be two numbers or two strings."};
    };
  }

  template <typename Op>
  auto number_op(Op const& op)
      -> std::function<Object(Object const&, Object const&)> {
    return [this, op](Object const& left, Object const& right) -> Object {
      Common::Utils::Operands::check_number_operand<Error>(chunk_->line_at(ip_),
                                                           left, right);

      return op(std::get<double>(left), std::get<double>(right));
    };
  }

  std::stack<Common::Types::Object> stack_;
  std::unordered_map<std::string, Common::Types::Object> globals_;

  Chunk const* chunk_;
  std::size_t ip_{0};
};

auto interpret(Chunk const& chunk) -> void {
  VirtualMachine vm{chunk};

  vm.run();
}

}  // namespace LOX::ByteCode::VM
