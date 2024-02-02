#include "./Cursor.hpp"

namespace LOX::Scanner {

Cursor::Cursor(std::string const& source)
    : source(source), start(0), current(0), line(1) {}

auto Cursor::advance() -> void { current++; }

auto Cursor::down() -> void { line++; }

auto Cursor::advance_word() -> void { start = current; }

auto Cursor::peek(std::size_t forward) const -> char {
  if (is_at_end()) return '\0';

  assert(current + forward < source.size());

  return source.at(current + forward);
}

auto Cursor::take() -> char {
  auto const tmp{peek()};
  advance();
  return tmp;
}

auto Cursor::peek_word() const -> std::string {
  return source.substr(start, current - start);
}

auto Cursor::at_line() const -> std::size_t { return line; }

auto Cursor::is_at_end() const -> bool { return current >= source.size(); }

auto Cursor::match(char const expected) const -> bool {
  if (is_at_end() || peek() != expected) {
    return false;
  };

  return true;
}

auto Cursor::index() const -> std::size_t { return 1 + current; }

}  // namespace LOX::Scanner
