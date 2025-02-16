#include <gtest/gtest.h>
#include "../deque.cpp"

TEST(DequeTests, DefaultConstructor) {
  Deque<int> deq;
  ASSERT_EQ(deq.size(), 0);
}

TEST(DequeTests, CopyConstructor) {
  Deque<int> deq;
  for (size_t i = 0; i < 1000; ++i) {
    deq.push_back(i + 1);
  }
  Deque<int> deq1 = deq;
  ASSERT_EQ(deq1.size(), deq.size());
  for (size_t i = 0; i < deq1.size(); ++i) {
    ASSERT_EQ(deq1[i], deq[i]);
  }
  ASSERT_EQ(deq1[0], deq[0]);
  deq1[0] = -1;
  ASSERT_NE(deq1[0], deq[0]);
  for (size_t i = 0; i < 1000; ++i) {
    deq1.push_front(i);
  }
  ASSERT_EQ(deq1.size(), 2000);
}

TEST(DequeTests, PushBack) {
  Deque<int> deq;
  for (size_t i = 0; i < 1000; ++i) {
    deq.push_back(i + 1);
  }
  ASSERT_EQ(deq.size(), 1000);
  for (size_t i = 0; i < deq.size(); ++i) {
    ASSERT_EQ(deq[i], i + 1);
  }
  ASSERT_EQ(deq.front(), 1);
  ASSERT_EQ(deq.back(), 1000);
}

TEST(DequeTests, PushFront) {
  Deque<int> deq;
  for (size_t i = 0; i < 1000; ++i) {
    deq.push_front(i);
  }
  ASSERT_EQ(deq.size(), 1000);
  size_t i = 0;
  for (size_t i = 0; i < 1000; --i) {
    ASSERT_EQ(deq[1000 - i - 1], i);
  }
}

TEST(DequeTests, PushBackFront) {
  Deque<int> deq;
  for (size_t i = 0; i < 1000; ++i) {
    deq.push_back(i);
  }
  ASSERT_EQ(deq.size(), 1000);
  for (size_t i = 0; i < 1000; ++i) {
    ASSERT_EQ(deq[i], i);
  }
  for (size_t i = 0; i < 1000; ++i) {
    deq.push_front(i);
  }
  ASSERT_EQ(deq.size(), 2000);
  size_t i = 0;
  for (size_t i = 0; i < 1000; --i) {
    ASSERT_EQ(deq[1000 - i - 1], i);
  }
  ASSERT_EQ(deq.front(), 999);
  ASSERT_EQ(deq.back(), 999);
}

TEST(DequeTests, PopBack) {
  Deque<int> deq;
  for (size_t i = 0; i < 1000; ++i) {
    deq.push_back(i);
  }
  for (size_t i = 0; i < 1000; ++i) {
    deq.pop_back();
  }
  ASSERT_EQ(deq.size(), 0);
}

TEST(DequeTests, PopFront) {
  Deque<int> deq;
  for (size_t i = 0; i < 1000; ++i) {
    deq.push_back(i);
  }
  for (size_t i = 0; i < 1000; ++i) {
    deq.pop_front();
  }
  ASSERT_EQ(deq.size(), 0);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}