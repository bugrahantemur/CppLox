#ifndef LOXUTILSARC
#define LOXUTILSARC

#include <memory>
#include <utility>

/**
 * A wrapper around shared_ptr.
 */
template <typename T>
class Arc {
  std::shared_ptr<T> impl;

 public:
  Arc() : impl(std::make_shared<T>()) {}

  Arc(T&& object) : impl(std::make_shared<T>(std::move(object))) {}

  Arc(T const& object) : impl(std::make_shared<T>(object)) {}

  Arc(Arc const& other) = default;
  Arc& operator=(Arc const& other) = default;
  Arc(Arc&& other) = default;
  Arc& operator=(Arc&& other) = default;
  ~Arc() = default;

  T& operator*() { return *impl; }
  T const& operator*() const { return *impl; }

  T* operator->() { return impl.get(); }
  T const* operator->() const { return impl.get(); }

  auto get() -> T* { return impl.get(); }
  auto get() const -> T const* { return impl.get(); }
};

#endif
