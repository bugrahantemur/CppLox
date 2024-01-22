#ifndef LOX_BUILTINS_CLOCK
#define LOX_BUILTINS_CLOCK

#include <chrono>
#include <string>
#include <vector>

#include "../types/object.hpp"
#include "./function_interface.hpp"

namespace LOX::Builtins {

struct Clock : public FunctionInterface {
  auto arity() const -> std::size_t final { return 0; }
  auto to_string() const -> std::string final { return "Clock"; };
  auto operator()(std::vector<Object> const& args) const -> Object final {
    static_cast<void>(args);

    return static_cast<double>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  };
};

}  // namespace LOX::Builtins

#endif
