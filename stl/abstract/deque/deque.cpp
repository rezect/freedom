#include <cstring>

#include "deque.hpp"

int DEFAULT_CAP = 10;

template <typename T, class Allocator>
Deque<T, Allocator>::Deque()
    : buckets_(nullptr), begin_(0, 0), end_(0, 0), size_(0), cap_(0) {}

template <typename T, class Allocator>
Deque<T, Allocator>::Deque(const Deque &other) {
  this->cap_ = other.cap_;
  buckets_ = (T **)malloc(sizeof(T *) * cap_);
  for (size_t i = 0; i < other.cap_; ++i) {
    if (other.buckets_[i] == nullptr) {
      buckets_[i] = nullptr;
    } else {
      buckets_[i] = alloc_.allocate(CHUNK_SZ);
      std::memcpy(buckets_[i], other.buckets_[i], sizeof(T) * CHUNK_SZ);
    }
  }
  this->begin_ = other.begin_;
  this->end_ = other.end_;
  this->size_ = other.size_;
}

template <typename T, class Allocator>
Deque<T, Allocator>::Deque(Deque &&other) noexcept {
  this->buckets_ = other.buckets_;
  this->begin_ = other.begin_;
  this->end_ = other.end_;
  this->size_ = other.size_;
  other.buckets_ = nullptr;
  other.begin_ = Deque<T, Allocator>::DequeIterator(0, 0);
  other.end_ = Deque<T, Allocator>::DequeIterator(0, 0);
  other.size_ = 0;
}

template <typename T, class Allocator>
Deque<T, Allocator> &Deque<T, Allocator>::operator=(const Deque &other) {
  if (this != &other) {
    this->clear();
    this->cap_ = other.cap_;
    buckets_ = (T **)malloc(sizeof(T *) * cap_);
    for (size_t i = 0; i < other.cap_; ++i) {
      if (other.buckets_[i] == nullptr) {
        buckets_[i] = nullptr;
      } else {
        buckets_[i] = alloc_.allocate(CHUNK_SZ);
        std::memcpy(buckets_[i], other.buckets_[i], sizeof(T) * CHUNK_SZ);
      }
    }
    this->begin_ = other.begin_;
    this->end_ = other.end_;
    this->size_ = other.size_;
  }
}

template <typename T, class Allocator>
Deque<T, Allocator> &Deque<T, Allocator>::operator=(Deque &&other) {
  if (this != &other) {
    this->clear();
    this->buckets_ = other.buckets_;
    this->begin_ = other.begin_;
    this->end_ = other.end_;
    this->size_ = other.size_;
    other.buckets_ = nullptr;
    other.begin_ = Deque<T, Allocator>::DequeIterator(0, 0);
    other.end_ = Deque<T, Allocator>::DequeIterator(0, 0);
    other.size_ = 0;
  }
}

template <typename T, class Allocator>
T &Deque<T, Allocator>::back() {
  if (size_ == 0) {
    throw DequeIsEmptyException("deque is empty");
  }
  auto it = end_;
  --it;
  return buckets_[it.row_][it.ind_];
}

template <typename T, class Allocator>
T &Deque<T, Allocator>::front() {
  if (size_ == 0) {
    throw DequeIsEmptyException("deque is empty");
  }
  return buckets_[begin_.row_][begin_.ind_];
}

template <typename T, class Allocator>
T &Deque<T, Allocator>::operator[](size_t pos) {
  size_t new_ind = pos % CHUNK_SZ + this->begin_.ind_;
  return buckets_[begin_.row_ + pos / CHUNK_SZ + new_ind / CHUNK_SZ]
                 [new_ind % CHUNK_SZ];
}

template <typename T, class Allocator> void Deque<T, Allocator>::clear() {
  if (size_ > 0) {
    auto it = begin_;
    while (it != end_) {
      alloc_.destroy(buckets_[it.row_] + it.ind_);
      ++it;
    }
  }
  for (size_t i = 0; i < cap_; ++i) {
    if (buckets_[i] == nullptr) {
      continue;
    }
    alloc_.deallocate(buckets_[i], CHUNK_SZ);
  }
  free(buckets_);
  this->begin_ = Deque<T, Allocator>::DequeIterator(0, 0);
  this->end_ = Deque<T, Allocator>::DequeIterator(0, 0);
  this->size_ = 0;
  this->cap_ = 0;
}

template <typename T, class Allocator> Deque<T, Allocator>::~Deque() {
  this->clear();
}

template <typename T, class Allocator>
size_t Deque<T, Allocator>::size() const {
  return this->size_;
}

template <typename T, class Allocator>
void Deque<T, Allocator>::restore(size_t new_cap, bool to_back) {
  if (to_back) {
    T** new_buckets = (T **)malloc(sizeof(T *) * new_cap);
    for (size_t i = 0; i < cap_; ++i) {
      new_buckets[i] = buckets_[i];
    }
    for (size_t i = cap_; i < new_cap; ++i) {
      new_buckets[i] = nullptr;
    }
    free(buckets_);
    this->buckets_ = new_buckets;
    this->cap_ = new_cap;
  } else {
    T **new_buckets = (T **)malloc(sizeof(T *) * new_cap);
    for (size_t i = 0; i < new_cap; ++i) {
      new_buckets[i] = nullptr;
    }
    std::memcpy(new_buckets + this->cap_, this->buckets_,
                sizeof(T *) * this->cap_);
    this->cap_ = new_cap;

    if (this->size_ > 0) {
      auto it = this->begin_;
      while (it != this->end_) {
        alloc_.destroy(buckets_[it.row_] + it.ind_);
        ++it;
      }
    }
    for (size_t i = 0; i < cap_; ++i) {
      if (buckets_[i] == nullptr) {
        continue;
      }
      alloc_.deallocate(buckets_[i], CHUNK_SZ);
    }
    free(buckets_);

    this->buckets_ = new_buckets;
    this->begin_.row_ += new_cap - cap_;
    this->end_.row_ += new_cap - cap_;
  }
}

template <typename T, class Allocator>
void Deque<T, Allocator>::push_back(const T &val) {
  this->emplace_back(val);
}

template <typename T, class Allocator>
void Deque<T, Allocator>::push_back(T &&val) {
  this->emplace_back(std::move(val));
}

template <typename T, class Allocator>
template <class... Args>
void Deque<T, Allocator>::emplace_back(Args &&...args) {
  if (buckets_ == nullptr) {
    this->cap_ = DEFAULT_CAP;
    this->buckets_ = (T **)malloc(sizeof(T *) * cap_);
    for (size_t i = 0; i < cap_; ++i) {
      buckets_[i] = nullptr;
      if (i == cap_ / 2) {
        buckets_[i] = alloc_.allocate(CHUNK_SZ);
      }
    }
    this->begin_ = Deque<T, Allocator>::DequeIterator(cap_ / 2, 0);
    this->end_ = Deque<T, Allocator>::DequeIterator(cap_ / 2, 1);
    alloc_.construct(buckets_[cap_ / 2], std::forward<Args>(args)...);
  } else {
    if (end_.row_ == cap_) {
      this->restore(cap_ * 2, true);
    }
    if (end_.ind_ == 0) {
      buckets_[end_.row_] = alloc_.allocate(CHUNK_SZ);
    }
    alloc_.construct(buckets_[end_.row_] + end_.ind_,
                     std::forward<Args>(args)...);
    ++end_;
  }
  ++size_;
}

// template <typename T, class Allocator>
// void Deque<T, Allocator>::push_front(const T &val) {
//   this->emplace_front(val);
// }

// template <typename T, class Allocator>
// void Deque<T, Allocator>::push_front(T &&val) {
//   this->emplace_front(std::move(val));
// }

// template <typename T, class Allocator>
// template <class... Args>
// void Deque<T, Allocator>::emplace_front(Args &&...args) {
//   if (buckets_ == nullptr) {
//     this->cap_ = DEFAULT_CAP;
//     this->buckets_ = (T **)malloc(sizeof(T *) * cap_);
//     for (size_t i = 0; i < cap_; ++i) {
//       buckets_[i] = nullptr;
//       if (i == cap_ / 2) {
//         buckets_[i] = alloc_.allocate(CHUNK_SZ);
//       }
//     }
//     this->begin_ = Deque<T, Allocator>::DequeIterator(cap_ / 2, 0);
//     this->end_ = Deque<T, Allocator>::DequeIterator(cap_ / 2, 1);
//     alloc_.construct(buckets_[cap_ / 2], std::forward<Args>(args)...);
//   } else {
//     if (begin_.row_ == 0) {
//       this->restore(cap_ * 2, false);
//       buckets_[begin_.row_] = alloc_.allocate(CHUNK_SZ);
//     }
//     alloc_.construct(buckets_[begin_.row_] + begin_.ind_,
//                      std::forward<Args>(args)...);
//     --begin_;
//   }
//   ++size_;
// }

// template <typename T, class Allocator>
// void Deque<T, Allocator>::pop_back() {
//   if (size_ == 0) {
//     throw DequeIsEmptyException("deque is empty");
//   }
//   alloc_.destroy(buckets_[end_.row_] + end_.ind_);
//   --end_;
//   --size_;
// }