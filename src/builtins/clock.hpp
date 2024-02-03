#ifndef LOX_BUILTINS_CLOCK
#define LOX_BUILTINS_CLOCK

#include <chrono>
#include <string>
#include <vector>

#include "../Types/Object/Builtins/FunctionInterface.hpp"
#include "../Types/Object/Object.hpp"

namespace LOX::Native {

struct Clock : public Types::Objects::Builtins::FunctionInterface {
  auto arity() const -> std::size_t final { return 0; }
  auto to_string() const -> std::string final { return "Clock"; };
  auto operator()(std::vector<Types::Objects::Object> const& args) const
      -> Types::Objects::Object final {
    static_cast<void>(args);

    return static_cast<double>(
        std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
  };
};

}  // namespace LOX::Native

#endif
