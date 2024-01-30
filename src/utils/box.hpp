#ifndef LOX_UTILS_BOX
#define LOX_UTILS_BOX

#include <memory>

/**
 * A wrapper around unique_ptr. Allows recursive data structures with value
 * semantics.
 */
template <typename T>
class Box {
  std::unique_ptr<T> impl_;

 public:
  Box(T&& object) : impl_(std::make_unique<T>(std::move(object))) {}
  Box(T const& object) : impl_(std::make_unique<T>(object)) {}

  Box(Box const& other) : Box(*other.impl_) {}
  Box& operator=(Box const& other) {
    *impl_ = *other.impl_;
    return *this;
  }

  Box(Box&& other) = default;
  Box& operator=(Box&& other) = default;
  ~Box() = default;

  T& operator*() { return *impl_; }
  T const& operator*() const { return *impl_; }

  T* operator->() { return impl_.get(); }
  T const* operator->() const { return impl_.get(); }
};

#endif
