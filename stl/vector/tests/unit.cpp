#include <gtest/gtest.h>

#include "allocator.hpp"
#include "vector.cpp"

TEST(VectorTest, Initialization) {
  Vector<int, Allocator<int>> v{1, 2, 3};
  EXPECT_EQ(v.Size(), 3);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
