#pragma once

#include <exception>
#include <string>

class VectorIsEmptyException : std::exception {
public:
  explicit VectorIsEmptyException(const std::string &text)
      : error_message_(text) {}

  const char *what() const noexcept override { return error_message_.data(); }

private:
  std::string_view error_message_;
};

class InvalidIndexException : std::exception {
public:
  explicit InvalidIndexException(const std::string &text)
      : error_message_(text) {}

  const char *what() const noexcept override { return error_message_.data(); }

private:
  std::string_view error_message_;
};

class InvalidReserveSizeException : std::exception {
public:
  explicit InvalidReserveSizeException(const std::string &text)
      : error_message_(text) {}

  const char *what() const noexcept override { return error_message_.data(); }

private:
  std::string_view error_message_;
};