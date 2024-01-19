#ifndef LOX_UTILS_ERROR
#define LOX_UTILS_ERROR

namespace LOX {

class ErrorInterface {
 public:
  virtual auto report() const -> void = 0;
};

}  // namespace LOX
#endif
