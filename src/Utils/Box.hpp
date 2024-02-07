#ifndef LOX_UTILS_BOX
#define LOX_UTILS_BOX

#include <memory>
#include <utility>

template <typename T>
class Box {
  std::unique_ptr<T> impl;

 public:
  Box() : impl{std::make_unique<T>()} {}

  Box(T&& object) : impl{std::make_unique<T>(std::move(object))} {}

  Box(T const& object) : impl{std::make_unique<T>(object)} {}

  Box(Box const& other) : Box{*other.impl} {}
  Box& operator=(Box const& other) {
    *impl = *other.impl;
    return *this;
  }

  Box(Box&& other) = default;
  auto operator=(Box&& other) -> Box& = default;
  ~Box() = default;

  auto operator*() -> T& { return *impl; }
  auto operator*() const -> T const& { return *impl; }

  auto operator->() -> T* { return impl.get(); }
  auto operator->() const -> T const* { return impl.get(); }

  auto get() -> T* { return impl.get(); }
  auto get() const -> T const* { return impl.get(); }
};

#endif
