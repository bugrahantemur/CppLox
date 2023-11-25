#ifndef CPPLOX_UTILS_PRINTER
#define CPPLOX_UTILS_PRINTER

namespace Printer {

template <typename OStream>
auto print_prompt_marker(OStream &out) -> void {
  out << "> ";
}

template <typename OStream>
auto print_too_many_arguments(OStream &out) -> void {
  out << "Too many arguments.\nUsage: cpplox [script]\n";
}
}  // namespace Printer

#endif
