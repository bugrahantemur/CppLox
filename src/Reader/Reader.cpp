#include "./Reader.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "./Error/Error.hpp"

namespace LOX::Reader {

auto read(std::string const &path) -> std::string {
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
