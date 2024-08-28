#ifndef LOX_BYTECODE_TYPES_HEAPOBJECT
#define LOX_BYTECODE_TYPES_HEAPOBJECT

#include <variant>

namespace LOX::ByteCode::Types {

using HeapObject = std::variant<std::string>;

}  // namespace LOX::ByteCode::Types

#endif
