#include "./Clock.hpp"

#include <chrono>
#include <string>
#include <unordered_map>
#include <vector>

#include "../../../../Common/Types/Objects/Object.hpp"
#include "../../../Types/Environment/Environment.hpp"

namespace LOX::TreeWalk::Native::Functions {

auto Clock::arity() const -> std::size_t { return 0; }
auto Clock::to_string() const -> std::string { return "Clock"; };
auto Clock::operator()(
    Arc<Types::Environment> const& environment,
    std::unordered_map<Common::Types::Token, std::size_t> const& resolution,
    std::vector<Common::Types::Object> const& args) const
    -> Common::Types::Object {
  static_cast<void>(environment);
  static_cast<void>(resolution);
  static_cast<void>(args);

  return static_cast<double>(
      std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count());
}

}  // namespace LOX::TreeWalk::Native::Functions
