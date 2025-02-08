#include <gtest/gtest.h>

#include "allocator.hpp"
#include "vector.cpp"

class VectorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    v.push_back(6);
    v.push_back(7);
    ASSERT_EQ(v.size(), 7);
  }
  vector<int, allocator<int>> v;
};

// Constructors tests

TEST(ConstructorsTests, DefaultConstructor) {
  vector<int, allocator<int>> vec;
  ASSERT_EQ(vec.size(), 0);
}

TEST(ConstructorsTests, SizeConstructor) {
  vector<int, allocator<int>> vec(5, 1);
  ASSERT_EQ(vec.size(), 5);
  for (size_t i = 0; i < vec.size(); ++i) {
    ASSERT_EQ(vec[i], 1);
  }
}

TEST(ConstructorsTests, InitializerListConstructor) {
  vector<int, allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.size(), 5);
  for (size_t i = 0; i < vec.size(); ++i) {
    ASSERT_EQ(vec[i], i + 1);
  }
}

TEST(ConstructorsTests, CopyConstructor) {
  vector<int, allocator<int>> vec1({1, 2, 3, 4, 5});
  vector<int, allocator<int>> vec2(vec1);
  ASSERT_EQ(vec1.size(), 5);
  ASSERT_EQ(vec1.size(), vec2.size());
  for (size_t i = 0; i < vec1.size(); ++i) {
    ASSERT_EQ(vec1[i], vec2[i]);
  }
}

TEST(ConstructorsTests, MoveConstructor) {
  vector<int, allocator<int>> vec1({1, 2, 3, 4, 5});
  vector<int, allocator<int>> vec2(std::move(vec1));
  ASSERT_EQ(vec1.size(), 0);
  ASSERT_EQ(vec2.size(), 5);
  for (size_t i = 0; i < vec2.size(); ++i) {
    ASSERT_EQ(vec2[i], i + 1);
  }
}

TEST(ConstructorsTests, CopyAssignment) {
  vector<int, allocator<int>> vec1({1, 2, 3, 4, 5});
  vector<int, allocator<int>> vec2;
  vec2 = vec1;
  ASSERT_EQ(vec1.size(), 5);
  ASSERT_EQ(vec1.size(), vec2.size());
  for (size_t i = 0; i < vec1.size(); ++i) {
    ASSERT_EQ(vec1[i], vec2[i]);
  }
}

TEST(ConstructorsTests, SelfAssignment) {
  vector<int, allocator<int>> vec1({1, 2, 3, 4, 5});
  vec1 = vec1;
  ASSERT_EQ(vec1.size(), 5);
}

TEST(ConstructorsTests, MoveAssignment) {
  vector<int, allocator<int>> vec1({1, 2, 3, 4, 5});
  vector<int, allocator<int>> vec2;
  vec2 = std::move(vec1);
  ASSERT_EQ(vec1.size(), 0);
  ASSERT_EQ(vec2.size(), 5);
  for (size_t i = 0; i < vec2.size(); ++i) {
    ASSERT_EQ(vec2[i], i + 1);
  }
}

// Vector tests

TEST(VectorTests, GetElementByPos) {
  vector<int, allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec[0], 1);
  ASSERT_EQ(vec[1], 2);
  ASSERT_EQ(vec[2], 3);
  ASSERT_EQ(vec[3], 4);
  ASSERT_EQ(vec[4], 5);
}

TEST(VectorTests, GetElementByWrongPos) {
  vector<int, allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_THROW(vec[10], InvalidIndexException);
  ASSERT_THROW(vec[-1], InvalidIndexException);
}

TEST(VectorTests, GetFront) {
  vector<int, allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.front(), 1);
}

TEST(VectorTests, GetBack) {
  vector<int, allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.back(), 5);
}

TEST(VectorTests, GetSize) {
  vector<int, allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.size(), 5);
}

TEST(VectorTests, GetData) {
  vector<int, allocator<int>> vec({1, 2, 3, 4, 5});
  int* data = vec.data();
  ASSERT_EQ(data[0], 1);
  ASSERT_EQ(data[1], 2);
  ASSERT_EQ(data[2], 3);
  ASSERT_EQ(data[3], 4);
  ASSERT_EQ(data[4], 5);
}

TEST(VectorTests, GetCapacity) {
  vector<int, allocator<int>> vec({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
  ASSERT_EQ(vec.capacity(), DEFAULT_CAPACITY * 2);
}

TEST(VectorTests, Clear) {
  vector<int, allocator<int>> vec({1, 2, 3, 4, 5});
  vec.clear();
  ASSERT_EQ(vec.size(), 0);
}

TEST(VectorTests, Insert) {
  vector<int, allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.size(), 5);
  vec.insert(0, 0);
  ASSERT_EQ(vec[0], 0);
  ASSERT_EQ(vec[1], 1);
  ASSERT_EQ(vec[2], 2);
  ASSERT_EQ(vec[3], 3);
  ASSERT_EQ(vec[4], 4);
  ASSERT_EQ(vec[5], 5);
  ASSERT_EQ(vec.size(), 6);
  vec.insert(3, 10);
  ASSERT_EQ(vec[0], 0);
  ASSERT_EQ(vec[1], 1);
  ASSERT_EQ(vec[2], 2);
  ASSERT_EQ(vec[3], 10);
  ASSERT_EQ(vec[4], 3);
  ASSERT_EQ(vec[5], 4);
  ASSERT_EQ(vec[6], 5);
  ASSERT_EQ(vec.size(), 7);
}

TEST(VectorTests, Erase) {
  vector<int, allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.size(), 5);
  vec.erase(0, 2);
  ASSERT_EQ(vec[0], 3);
  ASSERT_EQ(vec[1], 4);
  ASSERT_EQ(vec[2], 5);
  ASSERT_EQ(vec.size(), 3);
  vec.erase(0, 3);
  ASSERT_EQ(vec.size(), 0);
}

TEST(VectorTests, push_back) {
  vector<int, allocator<int>> vec;
  ASSERT_EQ(vec.size(), 0);
  vec.push_back(1);
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec[0], 1);
  vec.push_back(2);
  ASSERT_EQ(vec.size(), 2);
  ASSERT_EQ(vec[1], 2);
  vec.push_back(3);
  ASSERT_EQ(vec.size(), 3);
  ASSERT_EQ(vec[2], 3);
  vec.push_back(4);
  ASSERT_EQ(vec.size(), 4);
  ASSERT_EQ(vec[3], 4);
  vec.push_back(5);
  ASSERT_EQ(vec.size(), 5);
  ASSERT_EQ(vec[4], 5);
}

TEST(VectorTests, Emplaceback) {
  vector<int, allocator<int>> vec;
  ASSERT_EQ(vec.size(), 0);
  vec.emplace_back(1);
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec[0], 1);
}

TEST(VectorTests, PopBack) {
  vector<int, allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.size(), 5);
  vec.pop_back();
  ASSERT_EQ(vec.size(), 4);
  ASSERT_EQ(vec.back(), 4);
  vec.pop_back();
  ASSERT_EQ(vec.size(), 3);
  ASSERT_EQ(vec.back(), 3);
  vec.pop_back();
  ASSERT_EQ(vec.size(), 2);
  ASSERT_EQ(vec.back(), 2);
  vec.pop_back();
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec.back(), 1);
  vec.pop_back();
  ASSERT_EQ(vec.size(), 0);
}

TEST(VectorTests, Resize) {
  vector<int, allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.size(), 5);
  vec.resize(3, 0);
  ASSERT_EQ(vec.size(), 3);
  ASSERT_EQ(vec[0], 1);
  ASSERT_EQ(vec[1], 2);
  ASSERT_EQ(vec[2], 3);
  vec.resize(7, 0);
  ASSERT_EQ(vec.size(), 7);
  ASSERT_EQ(vec[3], 0);
  ASSERT_EQ(vec[4], 0);
  ASSERT_EQ(vec[5], 0);
  ASSERT_EQ(vec[6], 0);
}

TEST(VectorTests, Swap) {
  vector<int, allocator<int>> vec1({1, 2, 3, 4, 5});
  vector<int, allocator<int>> vec2({6, 7, 8, 9, 10});
  std::swap(vec1, vec2);
  ASSERT_EQ(vec1.size(), 5);
  ASSERT_EQ(vec1[0], 6);
  ASSERT_EQ(vec1[1], 7);
  ASSERT_EQ(vec1[2], 8);
  ASSERT_EQ(vec1[3], 9);
  ASSERT_EQ(vec1[4], 10);
  ASSERT_EQ(vec2.size(), 5);
  ASSERT_EQ(vec2[0], 1);
  ASSERT_EQ(vec2[1], 2);
  ASSERT_EQ(vec2[2], 3);
  ASSERT_EQ(vec2[3], 4);
  ASSERT_EQ(vec2[4], 5);
}

// Empty vector tests

TEST(EmptyVectorTests, ZeroSizeConstructor) {
  vector<int, allocator<int>> vec(0, 1);
  ASSERT_EQ(vec.size(), 0);
}

TEST(EmptyVectorTests, ZeroInitializerListConstructor) {
  vector<int, allocator<int>> vec({});
  ASSERT_EQ(vec.size(), 0);
}

TEST(EmptyVectorTests, ZeroCopyConstructor) {
  vector<int, allocator<int>> vec1;
  vector<int, allocator<int>> vec2(vec1);
  ASSERT_EQ(vec1.size(), 0);
  ASSERT_EQ(vec1.size(), vec2.size());
}

TEST(EmptyVectorTests, ZeroMoveConstructor) {
  vector<int, allocator<int>> vec1;
  vector<int, allocator<int>> vec2(std::move(vec1));
  ASSERT_EQ(vec1.size(), 0);
  ASSERT_EQ(vec2.size(), 0);
}

TEST(EmptyVectorTests, ZeroCopyAssignment) {
  vector<int, allocator<int>> vec1;
  vector<int, allocator<int>> vec2;
  vec2 = vec1;
  ASSERT_EQ(vec1.size(), 0);
  ASSERT_EQ(vec1.size(), vec2.size());
}

TEST(EmptyVectorTests, ZeroMoveAssignment) {
  vector<int, allocator<int>> vec1;
  vector<int, allocator<int>> vec2;
  vec2 = std::move(vec1);
  ASSERT_EQ(vec1.size(), 0);
  ASSERT_EQ(vec2.size(), 0);
}

TEST(EmptyVectorTests, GetFrontInEmptyVector) {
  vector<int, allocator<int>> vec;
  ASSERT_THROW(vec.front(), VectorIsEmptyException);
}

TEST(EmptyVectorTests, GetBackInEmptyVector) {
  vector<int, allocator<int>> vec;
  ASSERT_THROW(vec.back(), VectorIsEmptyException);
}

TEST(EmptyVectorTests, GetSizeInEmptyVector) {
  vector<int, allocator<int>> vec;
  ASSERT_EQ(vec.size(), 0);
}

TEST(EmptyVectorTests, GetDataInEmptyVector) {
  vector<int, allocator<int>> vec;
  int* data = vec.data();
  ASSERT_EQ(data, nullptr);
}

TEST(EmptyVectorTests, GetCapacityInEmptyVector) {
  vector<int, allocator<int>> vec;
  ASSERT_EQ(vec.capacity(), 0);
}

TEST(EmptyVectorTests, ClearInEmptyVector) {
  vector<int, allocator<int>> vec;
  vec.clear();
  ASSERT_EQ(vec.size(), 0);
}

TEST(EmptyVectorTests, InsertInEmptyVector) {
  vector<int, allocator<int>> vec;
  ASSERT_THROW(vec.insert(2, 1), InvalidIndexException);
  vec.insert(0, 1);
  ASSERT_EQ(vec.size(), 1);
  ASSERT_EQ(vec[0], 1);
}

TEST(EmptyVectorTests, EraseInEmptyVector) {
  vector<int, allocator<int>> vec;
  ASSERT_THROW(vec.erase(0, 1), InvalidIndexException);
}

// Custom type tests

class Employer {
 private:
  std::string name_;
  int age_;

 public:
  Employer(std::string name, int age) : name_(name), age_(age) {}

  std::string Name() const { return name_; }
  
  int Age() const { return age_; }

  ~Employer() {}
};

TEST(CustomTypeVectorTests, DefaultConstructor) {
  vector<Employer, allocator<Employer>> vec;
  ASSERT_EQ(vec.size(), 0);
}

TEST(CustomTypeVectorTests, InitializerListConstructor) {
  vector<Employer, allocator<Employer>> vec({Employer("John", 30), Employer("Jane", 25)});
  ASSERT_EQ(vec.size(), 2);
  ASSERT_EQ(vec[0].Name(), "John");
  ASSERT_EQ(vec[0].Age(), 30);
  ASSERT_EQ(vec[1].Name(), "Jane");
  ASSERT_EQ(vec[1].Age(), 25);
}

TEST(CustomTypeVectorTests, CopyConstructor) {
  vector<Employer, allocator<Employer>> vec1({Employer("John", 30), Employer("Jane", 25)});
  vector<Employer, allocator<Employer>> vec2(vec1);
  ASSERT_EQ(vec1.size(), 2);
  ASSERT_EQ(vec2.size(), 2);
  ASSERT_EQ(vec1[0].Name(), vec2[0].Name());
  ASSERT_EQ(vec1[0].Age(), vec2[0].Age());
  ASSERT_EQ(vec1[1].Name(), vec2[1].Name());
  ASSERT_EQ(vec1[1].Age(), vec2[1].Age());
}

TEST(CustomTypeVectorTests, MoveConstructor) {
  vector<Employer, allocator<Employer>> vec1({Employer("John", 30), Employer("Jane", 25)});
  vector<Employer, allocator<Employer>> vec2(std::move(vec1));
  ASSERT_EQ(vec1.size(), 0);
  ASSERT_EQ(vec2.size(), 2);
  ASSERT_EQ(vec2[0].Name(), "John");
  ASSERT_EQ(vec2[0].Age(), 30);
  ASSERT_EQ(vec2[1].Name(), "Jane");
  ASSERT_EQ(vec2[1].Age(), 25);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
