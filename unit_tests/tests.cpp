#include <gtest/gtest.h>

#include <fstream>
#include <string>

#include "../src/Common/Reader/Error/Error.hpp"
#include "../src/Common/Reader/Reader.hpp"

TEST(ReadFileTest, FileDoesNotExist) {
  // Arrange
  std::string const path{"nonexistent.lox"};

  // Act and Assert
  ASSERT_THROW(LOX::Reader::read(path), LOX::Reader::Error);
}
