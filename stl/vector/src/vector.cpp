#include "vector.hpp"
#include "exceptions.hpp"

template <typename T, class Allocate>
Vector<T, Allocate>::Vector() : arr_(nullptr), sz_(0), cap_(0) {}

template <typename T, class Allocate>
Vector<T, Allocate>::Vector(size_t count, const T& value) : Vector() {
  cap_ = DEFAULT_CAPACITY;
  while (cap_ < count) {
    cap_ *= 2;
  }
  arr_ = alloc_.allocate(cap_);
  for (size_t i = 0; i < count; ++i) {
    this->PushBack(value);
  }
}

template <typename T, class Allocate>
Vector<T, Allocate>::Vector(const Vector& other)
    : sz_(other.sz_), cap_(other.cap_) {
  if (cap_ == 0) {
    arr_ = nullptr;
    return;
  }
  arr_ = alloc_.allocate(other.cap_);
  std::uninitialized_copy(other.arr_, other.arr_ + other.sz_, arr_);
}

template <typename T, class Allocate>
Vector<T, Allocate>::Vector(Vector&& other) noexcept
    : arr_(other.arr_), sz_(other.sz_), cap_(other.cap_) {
  other.sz_ = 0;
  other.cap_ = 0;
  other.arr_ = nullptr;
}

template <typename T, class Allocate>
Vector<T, Allocate>::Vector(std::initializer_list<T> init)
    : arr_(nullptr), sz_(0), cap_(DEFAULT_CAPACITY) {
  arr_ = alloc_.allocate(cap_);
  for (const T& val : init) {
    this->PushBack(std::move(val));
  }
}

template <typename T, class Allocate>
Vector<T, Allocate>& Vector<T, Allocate>::operator=(const Vector& other) {
  if (this != &other) {
    this->Clear();
    sz_ = other.sz_;
    cap_ = other.cap_;
    if (cap_ == 0) {
      arr_ = nullptr;
      return *this;
    }
    arr_ = alloc_.allocate(cap_);
    std::uninitialized_copy(other.arr_, other.arr_ + other.sz_, arr_);
  }
  return *this;
}

template <typename T, class Allocate>
Vector<T, Allocate>& Vector<T, Allocate>::operator=(Vector&& other) {
  if (this != &other) {
    this->Clear();
    arr_ = other.arr_;
    sz_ = other.sz_;
    cap_ = other.cap_;
    other.cap_ = 0;
    other.sz_ = 0;
    other.arr_ = nullptr;
  }
  return *this;
}

template <typename T, class Allocate>
T& Vector<T, Allocate>::operator[](size_t pos) {
  if (pos >= sz_ || pos < 0) {
    throw InvalidIndexException("Invalid index");
  }
  return arr_[pos];
}

template <typename T, class Allocate>
T& Vector<T, Allocate>::Front() const {
  if (sz_ == 0) {
    throw VectorIsEmptyException("Vector is empty");
  }
  return arr_[0];
}

template <typename T, class Allocate>
bool Vector<T, Allocate>::IsEmpty() const noexcept {
  return sz_ == 0;
}

template <typename T, class Allocate>
T& Vector<T, Allocate>::Back() const {
  if (sz_ == 0) {
    throw VectorIsEmptyException("Vector is empty");
  }
  return arr_[sz_ - 1];
}

template <typename T, class Allocate>
T* Vector<T, Allocate>::Data() const noexcept {
  return arr_;
}

template <typename T, class Allocate>
size_t Vector<T, Allocate>::Size() const noexcept {
  return sz_;
}

template <typename T, class Allocate>
size_t Vector<T, Allocate>::Capacity() const noexcept {
  return cap_;
}

template <typename T, class Allocate>
void Vector<T, Allocate>::Reserve(size_t new_cap) {
  if (new_cap <= cap_) {
    InvalidReserveSizeException("New capacity is less or equal to current");
  }
  T* new_arr = alloc_.allocate(new_cap);
  size_t old_sz = sz_;
  for (size_t i = 0; i < sz_; ++i) {
    alloc_.construct(new_arr + i, std::move(arr_[i]));
  }
  this->Clear();
  sz_ = old_sz;
  arr_ = new_arr;
  cap_ = new_cap;
}

template <typename T, class Allocate>
void Vector<T, Allocate>::Clear() noexcept {
  if (cap_ == 0 || arr_ == nullptr) {
    return;
  }
  for (size_t i = 0; i < sz_; ++i) {
    alloc_.destroy(arr_ + i);
  }
  alloc_.deallocate(arr_, cap_);
  cap_ = 0;
  arr_ = nullptr;
  sz_ = 0;
}

template <typename T, class Allocate>
void Vector<T, Allocate>::Insert(size_t pos, T value) {
  if (pos > sz_ || pos < 0) {
    throw InvalidIndexException("Invalid index");
  }
  for (size_t i = pos; i < sz_; ++i) {
    std::swap(value, arr_[i]);
  }
  this->PushBack(std::move(value));
}

template <typename T, class Allocate>
void Vector<T, Allocate>::Erase(size_t begin_pos, size_t end_pos) {
  if (begin_pos >= end_pos || begin_pos > sz_ || end_pos > sz_) {
    throw InvalidIndexException("Invalid index");
  }
  T* new_arr = alloc_.allocate(cap_);
  size_t old_cap = cap_;
  size_t j = 0;
  for (size_t i = 0; i < begin_pos; ++i) {
    alloc_.construct(new_arr + j, std::move(arr_[i]));
    ++j;
  }
  for (size_t i = end_pos; i < sz_; ++i) {
    alloc_.construct(new_arr + j, std::move(arr_[i]));
    ++j;
  }
  this->Clear();
  arr_ = new_arr;
  sz_ = j;
  cap_ = old_cap;
}

template <typename T, class Allocate>
void Vector<T, Allocate>::PushBack(const T& value) {
  if (cap_ == 0) {
    this->Reserve(DEFAULT_CAPACITY);
  } else if (cap_ == sz_) {
    this->Reserve(cap_ * 2);
  }
  alloc_.construct(arr_ + sz_, value);
  ++sz_;
}

template <typename T, class Allocate>
void Vector<T, Allocate>::PushBack(T&& value) {
  this->EmplaceBack(std::move(value));
}

template <typename T, class Allocate>
template <class... Args>
void Vector<T, Allocate>::EmplaceBack(Args&&... args) {
  if (cap_ == 0) {
    this->Reserve(DEFAULT_CAPACITY);
  } else if (cap_ == sz_) {
    this->Reserve(cap_ * 2);
  }
  alloc_.construct(arr_ + sz_, std::forward<Args>(args)...);
  ++sz_;
}

template <typename T, class Allocate>
void Vector<T, Allocate>::PopBack() {
  if (sz_ == 0) {
    throw VectorIsEmptyException("You tried to pop from empty vector");
  }
  alloc_.destroy(arr_ + sz_ - 1);
  --sz_;
}

template <typename T, class Allocate>
void Vector<T, Allocate>::Resize(size_t count, const T& value) {
  if (count > sz_) {
    for (size_t i = sz_; i < count; ++i) {
      this->PushBack(value);
    }
  } else {
    this->Erase(count, sz_);
  }
}

template <typename T, class Allocate>
Vector<T, Allocate>::~Vector() {
  this->Clear();
}
