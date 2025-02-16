#include <gtest/gtest.h>
#include "../deque.cpp"

TEST(DequeTests, DefaultConstructor) {
  Deque<int> deq;
  ASSERT_EQ(deq.size(), 0);
}

TEST(DequeTests, PushBack) {
  Deque<int> deq;
  for (size_t i = 0; i < 1000000; ++i) {
    deq.push_back(i + 1);
  }
  ASSERT_EQ(deq.size(), 1000000);
  for (size_t i = 0; i < deq.size(); ++i) {
    ASSERT_EQ(deq[i], i + 1);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}