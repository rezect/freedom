#pragma once

#include <exception>
#include <string>

class vector_is_empty_exception : std::exception {
public:
  explicit vector_is_empty_exception(const std::string &text)
      : error_message_(text) {}

  const char *what() const noexcept override { return error_message_.data(); }

private:
  std::string_view error_message_;
};

class invalid_index_exception : std::exception {
public:
  explicit invalid_index_exception(const std::string &text)
      : error_message_(text) {}

  const char *what() const noexcept override { return error_message_.data(); }

private:
  std::string_view error_message_;
};

class invalid_reserve_size_exception : std::exception {
public:
  explicit invalid_reserve_size_exception(const std::string &text)
      : error_message_(text) {}

  const char *what() const noexcept override { return error_message_.data(); }

private:
  std::string_view error_message_;
};