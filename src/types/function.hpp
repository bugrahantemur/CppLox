#ifndef LOX_TYPES_FUNCTION
#define LOX_TYPES_FUNCTION

#include "./statement.hpp"

struct LoxFunction {
  auto arity() const -> std::size_t { return declaration_.params_.size(); }

  auto to_string() const -> std::string {
    return "<fn " + declaration_.name_ + ">";
  }

  FunctionStatement declaration_;
};

#endif
