#include "deque.hpp"

template <typename T, class Allocator>
Deque<T, Allocator>::Deque()
    : left_(nullptr), right_(nullptr), left_sz_(0), right_sz_(0), cap_(0) {}

template <typename T, class Allocator>
Deque<T, Allocator>::Deque(const Deque &other)
    : left_sz_(other.left_sz_), right_sz_(other.right_sz_), cap_(other.cap_) {
  left_ = (T **)malloc(sizeof(T *) * cap_);
  right_ = (T **)malloc(sizeof(T *) * cap_);
  for (size_t i = 0; i < left_sz_; ++i) {
    left_[i] = (T *)malloc(sizeof(T) * CHUNK_SZ);
    std::memcpy(left_[i], other.left_[i], sizeof(T) * CHUNK_SZ);
  }
  for (size_t i = 0; i < right_sz_; ++i) {
    right_[i] = (T *)malloc(sizeof(T) * CHUNK_SZ);
    std::memcpy(right_[i], other.right_[i], sizeof(T) * CHUNK_SZ);
  }
}

template <typename T, class Allocator>
Deque<T, Allocator>::Deque(Deque &&other) noexcept
    : left_sz_(other.left_sz_), right_sz_(other.right_sz_), cap_(other.cap_) {
  left_ = (T **)malloc(sizeof(T *) * cap_);
  right_ = (T **)malloc(sizeof(T *) * cap_);
  for (size_t i = 0; i < left_sz_; ++i) {
    left_[i] = (T *)malloc(sizeof(T) * CHUNK_SZ);
    for (size_t j = 0; j < CHUNK_SZ; ++j) {
      left_[i][j] = std::move(other.left_[i][j]);
    }
  }
  for (size_t i = 0; i < right_sz_; ++i) {
    right_[i] = (T *)malloc(sizeof(T) * CHUNK_SZ);
    for (size_t j = 0; j < CHUNK_SZ; ++j) {
      right_[i][j] = std::move(other.right_[i][j]);
    }
  }
  other.left_sz_ = 0;
  other.right_sz_ = 0;
  other.cap_ = 0;
  other.left_ = nullptr;
  other.right_ = nullptr;
}

template <typename T, class Allocator>
Deque<T, Allocator> &Deque<T, Allocator>::operator=(const Deque &other) {
  if (this != &other) {
    this->clear();
    left_sz_ = other.left_sz_;
    right_sz_ = other.right_sz_;
    for (size_t i = 0; i < left_sz_; ++i) {
      left_[i] = (T *)malloc(sizeof(T) * CHUNK_SZ);
      std::memcpy(left_[i], other.left_[i], sizeof(T) * CHUNK_SZ);
    }
    for (size_t i = 0; i < right_sz_; ++i) {
      right_[i] = (T *)malloc(sizeof(T) * CHUNK_SZ);
      std::memcpy(right_[i], other.right_[i], sizeof(T) * CHUNK_SZ);
    }
  }
  return *this;
}

template <typename T, class Allocator>
Deque<T, Allocator> &Deque<T, Allocator>::operator=(Deque &&other) {
  if (this != &other) {
    this->clear();
    left_sz_ = other.left_sz_;
    right_sz_ = other.right_sz_;
    for (size_t i = 0; i < left_sz_; ++i) {
      left_[i] = (T *)malloc(sizeof(T) * CHUNK_SZ);
      for (size_t j = 0; j < CHUNK_SZ; ++j) {
        left_[i][j] = std::move(other.left_[i][j]);
      }
    }
    for (size_t i = 0; i < right_sz_; ++i) {
      right_[i] = (T *)malloc(sizeof(T) * CHUNK_SZ);
      for (size_t j = 0; j < CHUNK_SZ; ++j) {
        right_[i][j] = std::move(other.right_[i][j]);
      }
    }
    other.left_sz_ = 0;
    other.right_sz_ = 0;
    other.cap_ = 0;
    other.left_ = nullptr;
    other.right_ = nullptr;
  }
  return *this;
}

template <typename T, class Allocator>
void Deque<T, Allocator>::push_back(T val) {
  if (cap_ == 0) {
    this->restore(DEFAULT_CAP);
    left_[0] = (T *)malloc(sizeof(T) * CHUNK_SZ);
    left_[0][0] = val;
    ++left_sz_;
    return;
  } else if (right_sz_ == CHUNK_SZ * cap_) {
    this->restore(cap_ * 2);
  } else if (right_sz_ + left_sz_ == 0) {
    left_[0] = (T *)malloc(sizeof(T) * CHUNK_SZ);
    left_[0][0] = val;
    ++left_sz_;
    return;
  }
  if (right_sz_ % CHUNK_SZ == 0) {
    right_[right_sz_ / CHUNK_SZ] = (T *)malloc(sizeof(T) * CHUNK_SZ);
  }
  right_[right_sz_ / CHUNK_SZ][right_sz_ % CHUNK_SZ] = val;
  ++right_sz_;
}

template <typename T, class Allocator>
void Deque<T, Allocator>::push_front(T val) {
  if (cap_ == 0) {
    this->restore(DEFAULT_CAP);
  } else if (left_sz_ == CHUNK_SZ * cap_) {
    this->restore(cap_ * 2);
  }
  if (left_sz_ % CHUNK_SZ == 0) {
    left_[left_sz_ / CHUNK_SZ] = (T *)malloc(sizeof(T) * CHUNK_SZ);
  }
  left_[left_sz_ / CHUNK_SZ][left_sz_ % CHUNK_SZ] = val;
  ++left_sz_;
}

template <typename T, class Allocator> void Deque<T, Allocator>::pop_back() {
  if (left_sz_ + right_sz_ == 0)
    throw DequeIsEmptyException("No such element");
  if (right_sz_ <= 1) {
    if (left_sz_ < 21) {
      this->to_center();
    } else {
      this->restore(cap_);
    }
  }
  if (right_sz_ + left_sz_ == 1) {
    ~T(left_[0][0]);
    free(left_[0]);
    --left_sz_;
    return;
  } else if (right_sz_ == 0) {
    this->to_center();
  }
  ~T(right_[(right_sz_ - 1) / CHUNK_SZ][(right_sz_ - 1) % CHUNK_SZ]);
  --right_sz_;
}

template <typename T, class Allocator> void Deque<T, Allocator>::pop_front() {
  if (left_sz_ + right_sz_ == 0)
    throw DequeIsEmptyException("No such element");
  if (left_sz_ == 1) {
    if (right_sz_ < 21) {
      this->to_center();
    } else {
      this->restore(cap_);
    }
  }
  ~T(left_[(left_sz_ - 1) / CHUNK_SZ][(left_sz_ - 1) % CHUNK_SZ]);
  --left_sz_;
}

template <typename T, class Allocator> T &Deque<T, Allocator>::back() {
  if (right_sz_ == 0) {
    if (left_sz_ == 0) {
      throw DequeIsEmptyException("No such element");
    } else {
      return left_[0][0];
    }
  }
  return right_[(right_sz_ - 1) / CHUNK_SZ][(right_sz_ - 1) % CHUNK_SZ];
}

template <typename T, class Allocator> T &Deque<T, Allocator>::front() {
  if (left_sz_ == 0) {
    throw DequeIsEmptyException("No such element");
  }
  return left_[(left_sz_ - 1) / CHUNK_SZ][(left_sz_ - 1) % CHUNK_SZ];
}

template <typename T, class Allocator>
T &Deque<T, Allocator>::operator[](size_t pos) {
  if (pos < left_sz_) {
    return left_[(left_sz_ - pos - 1) / CHUNK_SZ]
                [(left_sz_ - pos - 1) % CHUNK_SZ];
  } else {
    size_t remains = pos - left_sz_;
    return right_[remains / CHUNK_SZ][remains % CHUNK_SZ];
  }
}

template <typename T, class Allocator> void Deque<T, Allocator>::clear() {
  if (cap_ == 0)
    return;
  size_t left_chunks_count = left_sz_ / CHUNK_SZ;
  if (left_sz_ % CHUNK_SZ > 0)
    ++left_chunks_count;
  size_t right_chunks_count = right_sz_ / CHUNK_SZ;
  if (right_sz_ % CHUNK_SZ > 0)
    ++right_chunks_count;
  for (size_t i = 0; i < left_chunks_count; ++i) {
    for (size_t j = 0; j < CHUNK_SZ; ++j) {
      ~T(left_[i][j]);
    }
    free(left_[i]);
  }
  for (size_t i = 0; i < right_chunks_count; ++i) {
    for (size_t j = 0; j < CHUNK_SZ; ++j) {
      ~T(right_[i][j]);
    }
    free(right_[i]);
  }
  cap_ = 0;
  left_sz_ = 0;
  right_sz_ = 0;
  free(left_);
  free(right_);
  left_ = nullptr;
  right_ = nullptr;
}

template <typename T, class Allocator> size_t Deque<T, Allocator>::size() {
  return left_sz_ + right_sz_;
}

template <typename T, class Allocator> size_t Deque<T, Allocator>::capacity() {
  return cap_;
}

template <typename T, class Allocator>
void Deque<T, Allocator>::restore(size_t new_cap) {
  T **new_left = (T **)malloc(sizeof(T **) * new_cap);
  T **new_right = (T **)malloc(sizeof(T **) * new_cap);
  size_t left_chunks_count = left_sz_ / CHUNK_SZ;
  if (left_sz_ % CHUNK_SZ > 0)
    ++left_chunks_count;
  size_t right_chunks_count = right_sz_ / CHUNK_SZ;
  if (right_sz_ % CHUNK_SZ > 0)
    ++right_chunks_count;
  size_t sz = left_chunks_count + right_chunks_count;
  size_t half = sz / 2;
  if (left_chunks_count > half) {
    for (size_t i = left_chunks_count - half - 1; i < left_chunks_count; ++i) {
      new_left[i + half + 1 - left_chunks_count] = left_[i];
    }
    for (int i = left_chunks_count - half - 2; i > -right_chunks_count - 1;
         --i) {
      if (i >= 0) {
        new_right[left_chunks_count - half - 2 - i] = left_[i];
      } else {
        new_right[left_chunks_count - half - 2 - i] = right_[-1 - i];
      }
    }
    left_sz_ -= (left_chunks_count - half) * CHUNK_SZ;
    right_sz_ += (left_chunks_count - half) * CHUNK_SZ;
  } else if (right_chunks_count > half) {
    for (size_t i = right_chunks_count - half; i < right_chunks_count; ++i) {
      new_right[i + half - right_chunks_count] = right_[i];
    }
    for (int i = right_chunks_count - half - 1; i > -left_chunks_count - 1;
         --i) {
      if (i >= 0) {
        new_left[right_chunks_count - half - 1 - i] = right_[i];
      } else {
        new_left[right_chunks_count - half - 1 - i] = left_[-1 - i];
      }
    }
    left_sz_ += (right_chunks_count - half) * CHUNK_SZ;
    right_sz_ -= (right_chunks_count - half) * CHUNK_SZ;
  } else {
    for (size_t i = 0; i < left_chunks_count; ++i) {
      new_left[i] = left_[i];
    }
    for (size_t i = 0; i < right_chunks_count; ++i) {
      new_right[i] = right_[i];
    }
  }
  free(left_);
  free(right_);
  left_ = new_left;
  right_ = new_right;
  cap_ = new_cap;
}

template <typename T, class Allocator> void Deque<T, Allocator>::to_center() {
  T **new_left = (T **)malloc(sizeof(T **) * cap_);
  T **new_right = (T **)malloc(sizeof(T **) * cap_);
  size_t sz = left_sz_ + right_sz_;
  size_t sz_of_right_half = sz / 2;
  size_t sz_of_left_half = sz - sz_of_right_half;
  size_t new_left_sz = 0;
  size_t new_right_sz = 0;
  if (left_sz_ < right_sz_) {
    for (size_t i = sz_of_left_half; i > 0; --i) {
      if (i > left_sz_) {
        if (new_left_sz % CHUNK_SZ == 0) {
          new_left[new_left_sz / CHUNK_SZ] = (T *)malloc(sizeof(T) * CHUNK_SZ);
        }
        new_left[new_left_sz / CHUNK_SZ][new_left_sz % CHUNK_SZ] =
            right_[(right_sz_ - sz_of_right_half - (sz_of_left_half - i) - 1) /
                   CHUNK_SZ]
                  [(right_sz_ - sz_of_right_half - (sz_of_left_half - i) - 1) %
                   CHUNK_SZ];
        ++new_left_sz;
      } else {
        if (new_left_sz % CHUNK_SZ == 0) {
          new_left[new_left_sz / CHUNK_SZ] = (T *)malloc(sizeof(T) * CHUNK_SZ);
        }
        new_left[new_left_sz / CHUNK_SZ][new_left_sz % CHUNK_SZ] =
            left_[(i - 1) / CHUNK_SZ][(i - 1) % CHUNK_SZ];
        ++new_left_sz;
      }
    }
    for (size_t i = 0; i < sz_of_right_half; ++i) {
      if (new_right_sz % CHUNK_SZ == 0) {
        new_right[new_right_sz / CHUNK_SZ] = (T *)malloc(sizeof(T) * CHUNK_SZ);
      }
      new_right[new_right_sz / CHUNK_SZ][new_right_sz % CHUNK_SZ] =
          right_[(right_sz_ - new_left_sz + i) / CHUNK_SZ]
                [(right_sz_ - new_left_sz + i) % CHUNK_SZ];
      ++new_right_sz;
    }
  } else {
    for (size_t i = left_sz_ - sz_of_left_half; i < left_sz_; ++i) {
      if (new_left_sz % CHUNK_SZ == 0) {
        new_left[new_left_sz / CHUNK_SZ] = (T *)malloc(sizeof(T) * CHUNK_SZ);
      }
      new_left[new_left_sz / CHUNK_SZ][new_left_sz % CHUNK_SZ] =
          left_[i / CHUNK_SZ][i % CHUNK_SZ];
      ++new_left_sz;
    }
    for (size_t i = sz_of_right_half; i > 0; --i) {
      if (i > right_sz_) {
        if (new_right_sz % CHUNK_SZ == 0) {
          new_right[new_right_sz / CHUNK_SZ] =
              (T *)malloc(sizeof(T) * CHUNK_SZ);
        }
        new_right[new_right_sz / CHUNK_SZ][new_right_sz % CHUNK_SZ] =
            left_[(left_sz_ - sz_of_left_half - 1) / CHUNK_SZ]
                 [(left_sz_ - sz_of_left_half - 1) % CHUNK_SZ];
        ++new_right_sz;
      } else {
        if (new_right_sz % CHUNK_SZ == 0) {
          new_right[new_right_sz / CHUNK_SZ] =
              (T *)malloc(sizeof(T) * CHUNK_SZ);
        }
        new_right[new_right_sz / CHUNK_SZ][new_right_sz % CHUNK_SZ] =
            right_[(i - 1) / CHUNK_SZ][(i - 1) % CHUNK_SZ];
        ++new_right_sz;
      }
    }
  }
  free(left_);
  free(right_);
  left_ = new_left;
  right_ = new_right;
  left_sz_ = sz_of_left_half;
  right_sz_ = sz_of_right_half;
}

template <typename T, class Allocator> Deque<T, Allocator>::~Deque() {
  this->clear();
}