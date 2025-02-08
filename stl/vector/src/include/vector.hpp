#pragma once

#include <initializer_list>
#include <memory>

const int DEFAULT_CAPACITY = 10;

template <typename T, class Allocator = std::allocator<T>>
class Vector {
 public:
  Vector();

  Vector(size_t, const T&);

  Vector(const Vector&);

  Vector(Vector&&) noexcept;

  Vector& operator=(const Vector&);

  Vector& operator=(Vector&&);

  Vector(std::initializer_list<T>);

  T& operator[](size_t);

  T& Front() const;

  T& Back() const;

  T* Data() const noexcept;

  bool IsEmpty() const noexcept;

  size_t Size() const noexcept;

  size_t Capacity() const noexcept;

  void Reserve(size_t);

  void Clear() noexcept;

  void Insert(size_t, T);

  void Erase(size_t, size_t);

  void PushBack(const T&);

  void PushBack(T&&);

  template <class... Args>
  void EmplaceBack(Args&&...);

  void PopBack();

  void Resize(size_t, const T&);

  ~Vector();

 private:
  Allocator alloc_;
  T* arr_;
  size_t sz_;
  size_t cap_;
};

template <class Allocator>
class Vector<void*, Allocator> {
 public:
  Vector();

  void* Front() const;

  void* Back() const;

  void Reserve(size_t);

  void PushBack(void*);

  ~Vector();

 private:
  Allocator alloc_;
  void** arr_;
  size_t sz_;
  size_t cap_;
};