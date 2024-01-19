#ifndef LOX_UTILS_READER
#define LOX_UTILS_READER

#include <fstream>
#include <sstream>
#include <string>

namespace Reader {
/**
 * Reads the contents of a file and returns them as a string.
 *
 * @param path The path to the file to be read.
 *
 * @return The contents of the file as a string.
 *
 * @throws std::ifstream::failure If the file cannot be opened or read.
 */
inline auto read(std::string const &path) -> std::string {
  std::ifstream file(path);
  if (!file.is_open()) {
    throw std::ifstream::failure("Failed to open file");
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}
}  // namespace Reader

#endif
