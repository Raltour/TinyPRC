#include <gtest/gtest.h>

int add(int a, int b) {
  return a + b;
}

// TEST(测试套件名, 测试用例名)
TEST(MathTest, HandlesAddition) {
  EXPECT_EQ(add(1, 1), 2);
  EXPECT_EQ(add(10, 20), 30);
}
