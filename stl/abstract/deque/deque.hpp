#pragma once

#include <cstring>
#include <initializer_list>
#include <memory>
#include <mutex>
#include <utility>

#include "exceptions.hpp"

const int CHUNK_SZ = 10;
const int DEFAULT_CAP = 1;

template <typename T, class Allocator = std::allocator<T>>
class Deque {
 public:
  Deque();

  Deque(const Deque& other);

  Deque(Deque&& other) noexcept;

  Deque& operator=(const Deque& other);

  Deque& operator=(Deque&& other);

  ~Deque();

 public:
  // NOLINTNEXTLINE
  void push_back(T val);

  // NOLINTNEXTLINE
  void push_front(T val);

  // NOLINTNEXTLINE
  void pop_back();

  // NOLINTNEXTLINE
  void pop_front();

  // NOLINTNEXTLINE
  T& back();

  // NOLINTNEXTLINE
  T& front();

  T& operator[](size_t pos);

  // NOLINTNEXTLINE
  void clear();

  // NOLINTNEXTLINE
  size_t size();

  // NOLINTNEXTLINE
  size_t capacity();

 private:
  // NOLINTNEXTLINE
  void restore(size_t new_cap);

  // NOLINTNEXTLINE
  void to_center();

 private:
  T** left_;
  T** right_;
  size_t left_sz_;
  size_t right_sz_;
  size_t cap_;
};