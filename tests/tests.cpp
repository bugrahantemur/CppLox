#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "../src/utils/reader.h"

TEST(ReadFileTest, FileExists) {
  // Arrange
  std::string const path = "../tests/test.txt";

  // Act
  std::string const result = Reader::read_file(path);

  // Assert
  std::string const expected = "Hello, _world!\n4.20.floor() ; \"{x}\"\n";
  ASSERT_EQ(expected, result);
}

TEST(ReadFileTest, FileDoesNotExist) {
  // Arrange
  std::string const path = "nonexistent.txt";

  // Act and Assert
  ASSERT_THROW(Reader::read_file(path), std::ifstream::failure);
}

TEST(ReadFileTest, EmptyFile) {
  // Arrange
  std::string const path = "../tests/empty.txt";

  // Act
  std::string const result = Reader::read_file(path);

  // Assert
  std::string expected = "";
  ASSERT_EQ(expected, result);
}
