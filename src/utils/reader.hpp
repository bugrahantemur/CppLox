#ifndef LOX_UTILS_READER
#define LOX_UTILS_READER

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "./error.hpp"

namespace LOX::Reader {

class Error : public ErrorInterface {
 public:
  Error(std::string const &message)
      : ErrorInterface("File reader error: " + message) {}
};

/**
 * Reads the contents of a file and returns them as a string.
 *
 * @param path The path to the file to be read.
 *
 * @return The contents of the file as a string.
 *
 * @throws LOX::Reader::Error If the file cannot be opened or read.
 */
inline auto read(std::string const &path) -> std::string {
  std::string const extension{".lox"};

  if (path.find(extension) != path.size() - extension.size()) {
    throw Error("File must end in '" + extension + "'");
  }

  std::ifstream file(path);
  if (!file.is_open()) {
    throw Error("Failed to open '" + path + "'");
  }

  std::stringstream buffer;
  buffer << file.rdbuf();

  return buffer.str();
}
}  // namespace LOX::Reader

#endif
