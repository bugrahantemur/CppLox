#ifndef LOX_UTILS_ARCDYN
#define LOX_UTILS_ARCDYN

#include <memory>
#include <utility>

/**
 * A wrapper around shared_ptr.
 */
template <typename T>
class ArcDyn {
  std::shared_ptr<T> impl;

 public:
  template <typename U>
  ArcDyn(U&& object) : impl(std::make_shared<U>(std::forward<U>(object))) {}

  ArcDyn(ArcDyn const& other) = default;
  ArcDyn& operator=(ArcDyn const& other) = default;
  ArcDyn(ArcDyn&& other) = default;
  ArcDyn& operator=(ArcDyn&& other) = default;
  ~ArcDyn() = default;

  T& operator*() { return *impl; }
  T const& operator*() const { return *impl; }

  T* operator->() { return impl.get(); }
  T const* operator->() const { return impl.get(); }

  auto get() -> T* { return impl.get(); }
  auto get() const -> T const* { return impl.get(); }
};

#endif
