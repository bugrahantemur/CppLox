#ifndef LOX_TYPES_OBJECT
#define LOX_TYPES_OBJECT

#include <memory>
#include <string>
#include <variant>

#include "../utils/box.hpp"

using Object = std::variant<std::monostate, bool, double, std::string,
                            Box<struct LoxFunction>, Box<struct LoxClass>,
                            std::shared_ptr<class LoxInstance>>;

#endif
