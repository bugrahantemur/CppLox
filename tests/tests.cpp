#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "../src/Utils/reader.hpp"

TEST(ReadFileTest, FileExists) {
  // Arrange
  std::string const path{"../tests/test.txt"};

  // Act
  std::string const result{LOX::Reader::read(path)};

  // Assert
  std::string const expected{"Hello, _world!\n4.20.floor() ; \"{x}\"\n"};
  ASSERT_EQ(expected, result);
}

TEST(ReadFileTest, FileDoesNotExist) {
  // Arrange
  std::string const path{"nonexistent.lox"};

  // Act and Assert
  ASSERT_THROW(LOX::Reader::read(path), LOX::Reader::Error);
}

TEST(ReadFileTest, EmptyFile) {
  // Arrange
  std::string const path{"../tests/empty.lox"};

  // Act
  std::string const result{LOX::Reader::read(path)};

  // Assert
  std::string expected{""};
  ASSERT_EQ(expected, result);
}
