#include <gtest/gtest.h>

#include "allocator.hpp"
#include "vector.cpp"

class VectorTest : public ::testing::Test {
 protected:
  void SetUp() override {
    v.PushBack(1);
    v.PushBack(2);
    v.PushBack(3);
    v.PushBack(4);
    v.PushBack(5);
    v.PushBack(6);
    v.PushBack(7);
    ASSERT_EQ(v.Size(), 7);
  }
  Vector<int, Allocator<int>> v;
};

// Constructors tests

TEST(ConstructorsTests, DefaultConstructor) {
  Vector<int, Allocator<int>> vec;
  ASSERT_EQ(vec.Size(), 0);
}

TEST(ConstructorsTests, SizeConstructor) {
  Vector<int, Allocator<int>> vec(5, 1);
  ASSERT_EQ(vec.Size(), 5);
  for (size_t i = 0; i < vec.Size(); ++i) {
    ASSERT_EQ(vec[i], 1);
  }
}

TEST(ConstructorsTests, InitializerListConstructor) {
  Vector<int, Allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.Size(), 5);
  for (size_t i = 0; i < vec.Size(); ++i) {
    ASSERT_EQ(vec[i], i + 1);
  }
}

TEST(ConstructorsTests, CopyConstructor) {
  Vector<int, Allocator<int>> vec1({1, 2, 3, 4, 5});
  Vector<int, Allocator<int>> vec2(vec1);
  ASSERT_EQ(vec1.Size(), 5);
  ASSERT_EQ(vec1.Size(), vec2.Size());
  for (size_t i = 0; i < vec1.Size(); ++i) {
    ASSERT_EQ(vec1[i], vec2[i]);
  }
}

TEST(ConstructorsTests, MoveConstructor) {
  Vector<int, Allocator<int>> vec1({1, 2, 3, 4, 5});
  Vector<int, Allocator<int>> vec2(std::move(vec1));
  ASSERT_EQ(vec1.Size(), 0);
  ASSERT_EQ(vec2.Size(), 5);
  for (size_t i = 0; i < vec2.Size(); ++i) {
    ASSERT_EQ(vec2[i], i + 1);
  }
}

TEST(ConstructorsTests, CopyAssignment) {
  Vector<int, Allocator<int>> vec1({1, 2, 3, 4, 5});
  Vector<int, Allocator<int>> vec2;
  vec2 = vec1;
  ASSERT_EQ(vec1.Size(), 5);
  ASSERT_EQ(vec1.Size(), vec2.Size());
  for (size_t i = 0; i < vec1.Size(); ++i) {
    ASSERT_EQ(vec1[i], vec2[i]);
  }
}

TEST(ConstructorsTests, SelfAssignment) {
  Vector<int, Allocator<int>> vec1({1, 2, 3, 4, 5});
  vec1 = vec1;
  ASSERT_EQ(vec1.Size(), 5);
}

TEST(ConstructorsTests, MoveAssignment) {
  Vector<int, Allocator<int>> vec1({1, 2, 3, 4, 5});
  Vector<int, Allocator<int>> vec2;
  vec2 = std::move(vec1);
  ASSERT_EQ(vec1.Size(), 0);
  ASSERT_EQ(vec2.Size(), 5);
  for (size_t i = 0; i < vec2.Size(); ++i) {
    ASSERT_EQ(vec2[i], i + 1);
  }
}

// Vector tests

TEST(VectorTests, GetElementByPos) {
  Vector<int, Allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec[0], 1);
  ASSERT_EQ(vec[1], 2);
  ASSERT_EQ(vec[2], 3);
  ASSERT_EQ(vec[3], 4);
  ASSERT_EQ(vec[4], 5);
}

TEST(VectorTests, GetElementByWrongPos) {
  Vector<int, Allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_THROW(vec[10], InvalidIndexException);
  ASSERT_THROW(vec[-1], InvalidIndexException);
}

TEST(VectorTests, GetFront) {
  Vector<int, Allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.Front(), 1);
}

TEST(VectorTests, GetBack) {
  Vector<int, Allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.Back(), 5);
}

TEST(VectorTests, GetSize) {
  Vector<int, Allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.Size(), 5);
}

TEST(VectorTests, GetData) {
  Vector<int, Allocator<int>> vec({1, 2, 3, 4, 5});
  int* data = vec.Data();
  ASSERT_EQ(data[0], 1);
  ASSERT_EQ(data[1], 2);
  ASSERT_EQ(data[2], 3);
  ASSERT_EQ(data[3], 4);
  ASSERT_EQ(data[4], 5);
}

TEST(VectorTests, GetCapacity) {
  Vector<int, Allocator<int>> vec({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11});
  ASSERT_EQ(vec.Capacity(), DEFAULT_CAPACITY * 2);
}

TEST(VectorTests, Clear) {
  Vector<int, Allocator<int>> vec({1, 2, 3, 4, 5});
  vec.Clear();
  ASSERT_EQ(vec.Size(), 0);
}

TEST(VectorTests, Insert) {
  Vector<int, Allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.Size(), 5);
  vec.Insert(0, 0);
  ASSERT_EQ(vec[0], 0);
  ASSERT_EQ(vec[1], 1);
  ASSERT_EQ(vec[2], 2);
  ASSERT_EQ(vec[3], 3);
  ASSERT_EQ(vec[4], 4);
  ASSERT_EQ(vec[5], 5);
  ASSERT_EQ(vec.Size(), 6);
  vec.Insert(3, 10);
  ASSERT_EQ(vec[0], 0);
  ASSERT_EQ(vec[1], 1);
  ASSERT_EQ(vec[2], 2);
  ASSERT_EQ(vec[3], 10);
  ASSERT_EQ(vec[4], 3);
  ASSERT_EQ(vec[5], 4);
  ASSERT_EQ(vec[6], 5);
  ASSERT_EQ(vec.Size(), 7);
}

TEST(VectorTests, Erase) {
  Vector<int, Allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.Size(), 5);
  vec.Erase(0, 2);
  ASSERT_EQ(vec[0], 3);
  ASSERT_EQ(vec[1], 4);
  ASSERT_EQ(vec[2], 5);
  ASSERT_EQ(vec.Size(), 3);
  vec.Erase(0, 3);
  ASSERT_EQ(vec.Size(), 0);
}

TEST(VectorTests, PushBack) {
  Vector<int, Allocator<int>> vec;
  ASSERT_EQ(vec.Size(), 0);
  vec.PushBack(1);
  ASSERT_EQ(vec.Size(), 1);
  ASSERT_EQ(vec[0], 1);
  vec.PushBack(2);
  ASSERT_EQ(vec.Size(), 2);
  ASSERT_EQ(vec[1], 2);
  vec.PushBack(3);
  ASSERT_EQ(vec.Size(), 3);
  ASSERT_EQ(vec[2], 3);
  vec.PushBack(4);
  ASSERT_EQ(vec.Size(), 4);
  ASSERT_EQ(vec[3], 4);
  vec.PushBack(5);
  ASSERT_EQ(vec.Size(), 5);
  ASSERT_EQ(vec[4], 5);
}

TEST(VectorTests, Emplaceback) {
  Vector<int, Allocator<int>> vec;
  ASSERT_EQ(vec.Size(), 0);
  vec.EmplaceBack(1);
  ASSERT_EQ(vec.Size(), 1);
  ASSERT_EQ(vec[0], 1);
}

TEST(VectorTests, PopBack) {
  Vector<int, Allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.Size(), 5);
  vec.PopBack();
  ASSERT_EQ(vec.Size(), 4);
  ASSERT_EQ(vec.Back(), 4);
  vec.PopBack();
  ASSERT_EQ(vec.Size(), 3);
  ASSERT_EQ(vec.Back(), 3);
  vec.PopBack();
  ASSERT_EQ(vec.Size(), 2);
  ASSERT_EQ(vec.Back(), 2);
  vec.PopBack();
  ASSERT_EQ(vec.Size(), 1);
  ASSERT_EQ(vec.Back(), 1);
  vec.PopBack();
  ASSERT_EQ(vec.Size(), 0);
}

TEST(VectorTests, Resize) {
  Vector<int, Allocator<int>> vec({1, 2, 3, 4, 5});
  ASSERT_EQ(vec.Size(), 5);
  vec.Resize(3, 0);
  ASSERT_EQ(vec.Size(), 3);
  ASSERT_EQ(vec[0], 1);
  ASSERT_EQ(vec[1], 2);
  ASSERT_EQ(vec[2], 3);
  vec.Resize(7, 0);
  ASSERT_EQ(vec.Size(), 7);
  ASSERT_EQ(vec[3], 0);
  ASSERT_EQ(vec[4], 0);
  ASSERT_EQ(vec[5], 0);
  ASSERT_EQ(vec[6], 0);
}

TEST(VectorTests, Swap) {
  Vector<int, Allocator<int>> vec1({1, 2, 3, 4, 5});
  Vector<int, Allocator<int>> vec2({6, 7, 8, 9, 10});
  std::swap(vec1, vec2);
  ASSERT_EQ(vec1.Size(), 5);
  ASSERT_EQ(vec1[0], 6);
  ASSERT_EQ(vec1[1], 7);
  ASSERT_EQ(vec1[2], 8);
  ASSERT_EQ(vec1[3], 9);
  ASSERT_EQ(vec1[4], 10);
  ASSERT_EQ(vec2.Size(), 5);
  ASSERT_EQ(vec2[0], 1);
  ASSERT_EQ(vec2[1], 2);
  ASSERT_EQ(vec2[2], 3);
  ASSERT_EQ(vec2[3], 4);
  ASSERT_EQ(vec2[4], 5);
}

// Empty vector tests

TEST(EmptyVectorTests, ZeroSizeConstructor) {
  Vector<int, Allocator<int>> vec(0, 1);
  ASSERT_EQ(vec.Size(), 0);
}

TEST(EmptyVectorTests, ZeroInitializerListConstructor) {
  Vector<int, Allocator<int>> vec({});
  ASSERT_EQ(vec.Size(), 0);
}

TEST(EmptyVectorTests, ZeroCopyConstructor) {
  Vector<int, Allocator<int>> vec1;
  Vector<int, Allocator<int>> vec2(vec1);
  ASSERT_EQ(vec1.Size(), 0);
  ASSERT_EQ(vec1.Size(), vec2.Size());
}

TEST(EmptyVectorTests, ZeroMoveConstructor) {
  Vector<int, Allocator<int>> vec1;
  Vector<int, Allocator<int>> vec2(std::move(vec1));
  ASSERT_EQ(vec1.Size(), 0);
  ASSERT_EQ(vec2.Size(), 0);
}

TEST(EmptyVectorTests, ZeroCopyAssignment) {
  Vector<int, Allocator<int>> vec1;
  Vector<int, Allocator<int>> vec2;
  vec2 = vec1;
  ASSERT_EQ(vec1.Size(), 0);
  ASSERT_EQ(vec1.Size(), vec2.Size());
}

TEST(EmptyVectorTests, ZeroMoveAssignment) {
  Vector<int, Allocator<int>> vec1;
  Vector<int, Allocator<int>> vec2;
  vec2 = std::move(vec1);
  ASSERT_EQ(vec1.Size(), 0);
  ASSERT_EQ(vec2.Size(), 0);
}

TEST(EmptyVectorTests, GetFrontInEmptyVector) {
  Vector<int, Allocator<int>> vec;
  ASSERT_THROW(vec.Front(), VectorIsEmptyException);
}

TEST(EmptyVectorTests, GetBackInEmptyVector) {
  Vector<int, Allocator<int>> vec;
  ASSERT_THROW(vec.Back(), VectorIsEmptyException);
}

TEST(EmptyVectorTests, GetSizeInEmptyVector) {
  Vector<int, Allocator<int>> vec;
  ASSERT_EQ(vec.Size(), 0);
}

TEST(EmptyVectorTests, GetDataInEmptyVector) {
  Vector<int, Allocator<int>> vec;
  int* data = vec.Data();
  ASSERT_EQ(data, nullptr);
}

TEST(EmptyVectorTests, GetCapacityInEmptyVector) {
  Vector<int, Allocator<int>> vec;
  ASSERT_EQ(vec.Capacity(), 0);
}

TEST(EmptyVectorTests, ClearInEmptyVector) {
  Vector<int, Allocator<int>> vec;
  vec.Clear();
  ASSERT_EQ(vec.Size(), 0);
}

TEST(EmptyVectorTests, InsertInEmptyVector) {
  Vector<int, Allocator<int>> vec;
  ASSERT_THROW(vec.Insert(2, 1), InvalidIndexException);
  vec.Insert(0, 1);
  ASSERT_EQ(vec.Size(), 1);
  ASSERT_EQ(vec[0], 1);
}

TEST(EmptyVectorTests, EraseInEmptyVector) {
  Vector<int, Allocator<int>> vec;
  ASSERT_THROW(vec.Erase(0, 1), InvalidIndexException);
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
  Vector<Employer, Allocator<Employer>> vec;
  ASSERT_EQ(vec.Size(), 0);
}

TEST(CustomTypeVectorTests, InitializerListConstructor) {
  Vector<Employer, Allocator<Employer>> vec({Employer("John", 30), Employer("Jane", 25)});
  ASSERT_EQ(vec.Size(), 2);
  ASSERT_EQ(vec[0].Name(), "John");
  ASSERT_EQ(vec[0].Age(), 30);
  ASSERT_EQ(vec[1].Name(), "Jane");
  ASSERT_EQ(vec[1].Age(), 25);
}

TEST(CustomTypeVectorTests, CopyConstructor) {
  Vector<Employer, Allocator<Employer>> vec1({Employer("John", 30), Employer("Jane", 25)});
  Vector<Employer, Allocator<Employer>> vec2(vec1);
  ASSERT_EQ(vec1.Size(), 2);
  ASSERT_EQ(vec2.Size(), 2);
  ASSERT_EQ(vec1[0].Name(), vec2[0].Name());
  ASSERT_EQ(vec1[0].Age(), vec2[0].Age());
  ASSERT_EQ(vec1[1].Name(), vec2[1].Name());
  ASSERT_EQ(vec1[1].Age(), vec2[1].Age());
}

TEST(CustomTypeVectorTests, MoveConstructor) {
  Vector<Employer, Allocator<Employer>> vec1({Employer("John", 30), Employer("Jane", 25)});
  Vector<Employer, Allocator<Employer>> vec2(std::move(vec1));
  ASSERT_EQ(vec1.Size(), 0);
  ASSERT_EQ(vec2.Size(), 2);
  ASSERT_EQ(vec2[0].Name(), "John");
  ASSERT_EQ(vec2[0].Age(), 30);
  ASSERT_EQ(vec2[1].Name(), "Jane");
  ASSERT_EQ(vec2[1].Age(), 25);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
