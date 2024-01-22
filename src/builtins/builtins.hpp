#ifndef LOX_BUILTINS
#define LOX_BUILTINS

#include <chrono>
#include <string>
#include <vector>

#include "../types/object.hpp"
#include "./names.hpp"

namespace LOX::Builtins {

struct FunctionInterface {
  virtual auto arity() const -> std::size_t = 0;
  virtual auto to_string() const -> std::string = 0;
  virtual auto operator()(std::vector<Object> const& args) const -> Object = 0;
};

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

inline auto builtins() -> std::vector<std::pair<std::string, Object>> {
  return {{"clock", std::make_shared<Clock>()}, {"pi", 3.14159265359}};
}

}  // namespace LOX::Builtins

#endif
