#pragma once

#include <initializer_list>
#include <memory>

const int DEFAULT_CAPACITY = 10;

template <typename T, class allocator = std::allocator<T>>
class vector {
 public:
  class vector_iterator {
    friend class vector;

    using iterator_category = std::random_access_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;
    using size_type = size_t;

  public:
    explicit vector_iterator(pointer ptr = nullptr) : ptr_(ptr) {}

    // Операторы доступа
    reference operator*() const { return *ptr_; }
    pointer operator->() const { return ptr_; }

    // Операторы перемещения
    vector_iterator& operator++() {
      ++ptr_;
      return *this;
    }

    vector_iterator operator++(int) {
      vector_iterator tmp = *this; ++ptr_;
      return tmp;
    }

    vector_iterator& operator--() {
      --ptr_;
      return *this;
    }

    vector_iterator operator--(int) {
      vector_iterator tmp = *this; --ptr_;
      return tmp;
    }

    vector_iterator operator+(size_type n) {
      return vector_iterator(ptr_ + n);
    }

    vector_iterator operator-(size_type n) {
      return vector_iterator(ptr_ - n);
    }

    vector_iterator& operator+=(size_type n) {
      ptr_ += n;
      return *this;
    }

    vector_iterator& operator-=(size_type n) {
      ptr_ -= n;
      return *this;
    }

    // Операторы сравнения
    bool operator==(const vector_iterator& other) const {
      return ptr_ == other.ptr_;
    }

    bool operator!=(const vector_iterator& other) const {
      return !(ptr_ == other.ptr_);
    }

    // Оператор индексации
    reference operator[](size_t n) const {
      return *(ptr_ + n);
    }

   private:
    pointer ptr_;
  };

 public:
  vector();

  vector(size_t, const T&);

  vector(const vector&);

  vector(vector&&) noexcept;

  vector& operator=(const vector&);

  vector& operator=(std::initializer_list<T> ilist);

  vector& operator=(vector&&);

  vector(std::initializer_list<T>);

  T& at(size_t pos) const;

  T& operator[](size_t);

  T& front() const;

  T& back() const;

  vector_iterator begin() const {
    return vector_iterator(arr_);
  }

  vector_iterator end() const {
    return vector_iterator(arr_ + sz_);
  }

  T* data() const noexcept;

  bool is_empty() const noexcept;

  size_t size() const noexcept;

  size_t capacity() const noexcept;

  void reserve(size_t);

  void clear() noexcept;

  void insert(size_t, T);

  void erase(size_t, size_t);

  void push_back(const T&);

  void push_back(T&&);

  template <class... Args>
  void emplace_back(Args&&...);

  void pop_back();

  void resize(size_t, const T&);

  ~vector();

 private:
  allocator alloc_;
  T* arr_;
  size_t sz_;
  size_t cap_;
};

template <class allocator>
class vector<void*, allocator> {
 public:
  vector();

  void* front() const;

  void* back() const;

  void reserve(size_t);

  void push_back(void*);

  ~vector();

 private:
  allocator alloc_;
  void** arr_;
  size_t sz_;
  size_t cap_;
};