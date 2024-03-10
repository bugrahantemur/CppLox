
#ifndef LOX_READER
#define LOX_READER

#include <string>

namespace LOX::Reader {

/**
 * Reads the contents of a file and returns them as a string.
 *
 * @param path The path to the file to be read.
 *
 * @return The contents of the file as a string.
 *
 * @throws LOX::Reader::Error If the file cannot be opened or read.
 */
auto read(std::string const &path) -> std::string;

}  // namespace LOX::Reader

#endif
