#include <type_traits>

// Реализация std::forward
template <typename T>
T&& forward(std::remove_reference_t<T>& x) {
  return static_cast<T&&>(x);
}

// Реализация std::move
template <typename T>
std::remove_reference_t<T>&& move(T&& x) {
  return static_cast<std::remove_reference_t<T>&&>(x);
}