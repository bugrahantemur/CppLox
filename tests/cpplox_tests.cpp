#include <gtest/gtest.h>

TEST(CppLoxTest, BasicAssertions) {
  EXPECT_STRNE("hello", "world");

  EXPECT_EQ(7 * 6, 42);
}
