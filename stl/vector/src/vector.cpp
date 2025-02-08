#include "vector.hpp"
#include "exceptions.hpp"

template <typename T, class allocator>
vector<T, allocator>::vector() : arr_(nullptr), sz_(0), cap_(0) {}

template <typename T, class allocator>
vector<T, allocator>::vector(size_t count, const T& value) : vector() {
  cap_ = DEFAULT_CAPACITY;
  while (cap_ < count) {
    cap_ *= 2;
  }
  arr_ = alloc_.allocate(cap_);
  for (size_t i = 0; i < count; ++i) {
    this->push_back(value);
  }
}

template <typename T, class allocator>
vector<T, allocator>::vector(const vector& other)
    : sz_(other.sz_), cap_(other.cap_) {
  if (cap_ == 0) {
    arr_ = nullptr;
    return;
  }
  arr_ = alloc_.allocate(other.cap_);
  std::uninitialized_copy(other.arr_, other.arr_ + other.sz_, arr_);
}

template <typename T, class allocator>
vector<T, allocator>::vector(vector&& other) noexcept
    : arr_(other.arr_), sz_(other.sz_), cap_(other.cap_) {
  other.sz_ = 0;
  other.cap_ = 0;
  other.arr_ = nullptr;
}

template <typename T, class allocator>
vector<T, allocator>::vector(std::initializer_list<T> ilist)
    : arr_(nullptr), sz_(0), cap_(DEFAULT_CAPACITY) {
  arr_ = alloc_.allocate(cap_);
  for (const T& val : ilist) {
    this->push_back(std::move(val));
  }
}

template <typename T, class allocator>
vector<T, allocator>& vector<T, allocator>::operator=(const vector& other) {
  if (this != &other) {
    this->clear();
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

template <typename T, class allocator>
vector<T, allocator>& vector<T, allocator>::operator=(std::initializer_list<T> ilist) {
  this->clear();
  for (const T& val : ilist) {
    this->push_back(std::move(val));
  }
  return *this;
}

template <typename T, class allocator>
vector<T, allocator>& vector<T, allocator>::operator=(vector&& other) {
  if (this != &other) {
    this->clear();
    arr_ = other.arr_;
    sz_ = other.sz_;
    cap_ = other.cap_;
    other.cap_ = 0;
    other.sz_ = 0;
    other.arr_ = nullptr;
  }
  return *this;
}

template <typename T, class allocator>
T& vector<T, allocator>::at(size_t pos) const {
  if (pos >= sz_ || pos < 0) {
    throw invalid_index_exception("Invalid index");
  }
  return arr_[pos];
}

template <typename T, class allocator>
T& vector<T, allocator>::operator[](size_t pos) {
  return arr_[pos];
}

template <typename T, class allocator>
T& vector<T, allocator>::front() const {
  if (sz_ == 0) {
    throw vector_is_empty_exception("vector is empty");
  }
  return arr_[0];
}

template <typename T, class allocator>
bool vector<T, allocator>::is_empty() const noexcept {
  return sz_ == 0;
}

template <typename T, class allocator>
T& vector<T, allocator>::back() const {
  if (sz_ == 0) {
    throw vector_is_empty_exception("vector is empty");
  }
  return arr_[sz_ - 1];
}

template <typename T, class allocator>
T* vector<T, allocator>::data() const noexcept {
  return arr_;
}

template <typename T, class allocator>
size_t vector<T, allocator>::size() const noexcept {
  return sz_;
}

template <typename T, class allocator>
size_t vector<T, allocator>::capacity() const noexcept {
  return cap_;
}

template <typename T, class allocator>
void vector<T, allocator>::reserve(size_t new_cap) {
  if (new_cap <= cap_) {
    invalid_reserve_size_exception("New capacity is less or equal to current");
  }
  T* new_arr = alloc_.allocate(new_cap);
  size_t old_sz = sz_;
  for (size_t i = 0; i < sz_; ++i) {
    alloc_.construct(new_arr + i, std::move(arr_[i]));
  }
  this->clear();
  sz_ = old_sz;
  arr_ = new_arr;
  cap_ = new_cap;
}

template <typename T, class allocator>
void vector<T, allocator>::clear() noexcept {
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

template <typename T, class allocator>
void vector<T, allocator>::insert(size_t pos, T value) {
  if (pos > sz_ || pos < 0) {
    throw invalid_index_exception("Invalid index");
  }
  for (size_t i = pos; i < sz_; ++i) {
    std::swap(value, arr_[i]);
  }
  this->push_back(std::move(value));
}

template <typename T, class allocator>
void vector<T, allocator>::erase(size_t begin_pos, size_t end_pos) {
  if (begin_pos >= end_pos || begin_pos > sz_ || end_pos > sz_) {
    throw invalid_index_exception("Invalid index");
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
  this->clear();
  arr_ = new_arr;
  sz_ = j;
  cap_ = old_cap;
}

template <typename T, class allocator>
void vector<T, allocator>::push_back(const T& value) {
  if (cap_ == 0) {
    this->reserve(DEFAULT_CAPACITY);
  } else if (cap_ == sz_) {
    this->reserve(cap_ * 2);
  }
  alloc_.construct(arr_ + sz_, value);
  ++sz_;
}

template <typename T, class allocator>
void vector<T, allocator>::push_back(T&& value) {
  this->emplace_back(std::move(value));
}

template <typename T, class allocator>
template <class... Args>
void vector<T, allocator>::emplace_back(Args&&... args) {
  if (cap_ == 0) {
    this->reserve(DEFAULT_CAPACITY);
  } else if (cap_ == sz_) {
    this->reserve(cap_ * 2);
  }
  alloc_.construct(arr_ + sz_, std::forward<Args>(args)...);
  ++sz_;
}

template <typename T, class allocator>
void vector<T, allocator>::pop_back() {
  if (sz_ == 0) {
    throw vector_is_empty_exception("You tried to pop from empty vector");
  }
  alloc_.destroy(arr_ + sz_ - 1);
  --sz_;
}

template <typename T, class allocator>
void vector<T, allocator>::resize(size_t count, const T& value) {
  if (count > sz_) {
    for (size_t i = sz_; i < count; ++i) {
      this->push_back(value);
    }
  } else {
    this->erase(count, sz_);
  }
}

template <typename T, class allocator>
vector<T, allocator>::~vector() {
  this->clear();
}

// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------
// ------------------------------------------------------------------------------------------

template <class allocator>
vector<void*, allocator>::vector() : arr_(nullptr), sz_(0), cap_(0) {}

template <class allocator>
void* vector<void*, allocator>::front() const {
  if (sz_ == 0) {
    throw vector_is_empty_exception("vector is empty");
  }
  return arr_[0];
}

template <class allocator>
void* vector<void*, allocator>::back() const {
  if (sz_ == 0) {
    throw vector_is_empty_exception("vector is empty");
  }
  return arr_[sz_ - 1];
}

template <class allocator>
void vector<void*, allocator>::reserve(size_t new_cap) {
  if (new_cap <= cap_) {
    return;
  }
  void** new_arr = static_cast<void**>(alloc_.allocate(new_cap));
  for (size_t i = 0; i < sz_; ++i) {
    new_arr[i] = arr_[i];
  }
  alloc_.deallocate(arr_, cap_);
  arr_ = new_arr;
  cap_ = new_cap;
}

template <class allocator>
void vector<void*, allocator>::push_back(void* value) {
  if (cap_ == 0) {
    this->reserve(DEFAULT_CAPACITY);
  } else if (cap_ == sz_) {
    this->reserve(cap_ * 2);
  }
  arr_[sz_] = value;
  ++sz_;
}

template <class allocator>
vector<void*, allocator>::~vector() {
  alloc_.deallocate(arr_, cap_);
}
