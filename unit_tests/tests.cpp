#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "../src/Reader/Error/Error.hpp"
#include "../src/Reader/Reader.hpp"

TEST(ReadFileTest, FileDoesNotExist) {
  // Arrange
  std::string const path{"nonexistent.lox"};

  // Act and Assert
  ASSERT_THROW(LOX::Reader::read(path), LOX::Reader::Error);
}
