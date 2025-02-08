#include <utility>
#include <cstdlib>

template<typename T>
class allocator {
 public:
  T* allocate(size_t count) const {
    return (T*)(::operator new(count * sizeof(T)));
  }

  void deallocate(T* ptr, size_t /*count*/) {
    ::operator delete(ptr);
  }

  template <typename... Args>
  void construct(T* ptr, Args&&... args) {
    new(ptr) T(std::forward<Args>(args)...);
  }

  void destroy(T* ptr) {
    ptr->~T();
  }
};