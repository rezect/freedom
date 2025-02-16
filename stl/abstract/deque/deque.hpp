#pragma once

#include <vector>

#include "exceptions.hpp"

const int CHUNK_SZ = 32;

template <typename T, class Allocator = std::allocator<T>> class Deque {
public:
  Deque();

  Deque(const Deque &other);

  Deque(Deque &&other) noexcept;

  Deque &operator=(const Deque &other);

  Deque &operator=(Deque &&other);

  ~Deque();

public:
  class DequeIterator {
  public:
    // NOLINTNEXTLINE
    using value_type = T;
    // NOLINTNEXTLINE
    using reference_type = value_type &;
    // NOLINTNEXTLINE
    using pointer_type = value_type *;
    // NOLINTNEXTLINE
    using difference_type = std::ptrdiff_t;
    // NOLINTNEXTLINE
    using iterator_category = std::random_access_iterator_tag;

    inline bool operator==(const DequeIterator &other) const {
      return this->row_ == other.row_ &&
             this->ind_ == other.ind_;
    }

    inline bool operator!=(const DequeIterator &other) const {
      return this->row_ != other.row_ ||
             this->ind_ != other.ind_;
    }

    DequeIterator &operator++() {
      if (ind_ == CHUNK_SZ - 1) {
        ++row_;
        ind_ = 0;
      } else {
        ++ind_;
      }
      return *this;
    }

    DequeIterator operator++(int) {
      DequeIterator new_iter(row_, ind_);
      ++(*this);
      return new_iter;
    };

    DequeIterator &operator--() {
      if (ind_ == 0) {
        --row_;
        ind_ = CHUNK_SZ - 1;
      } else {
        --ind_;
      }
      return *this;
    }

    DequeIterator operator--(int) {
      DequeIterator new_iter(row_, ind_);
      --(*this);
      return new_iter;
    };

  private:
    friend class Deque<T>;
    explicit DequeIterator(size_t row_,
                           size_t ind_)
        : row_(row_),
          ind_(ind_) {}

  private:
    size_t row_;
    size_t ind_;
  };

  DequeIterator begin() const { return this->begin_; }

  DequeIterator end() const { return this->end_; }

  void push_back(const T &);

  void push_back(T &&);

  template <class... Args> void emplace_back(Args &&...);

  void push_front(const T &);

  void push_front(T &&);

  template <class... Args> void emplace_front(Args &&...);

  void pop_back();

  void pop_front();

  T &back();

  T &front();

  T &operator[](size_t);

  void clear();

  size_t size() const;

private:
  void restore(size_t, bool);

private:
  T **buckets_;
  DequeIterator begin_;
  DequeIterator end_;
  size_t size_;
  size_t cap_;
  Allocator alloc_;
};