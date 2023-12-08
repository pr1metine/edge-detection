#pragma once

#include <ostream>
#include <vector>

namespace image {

template <typename T>
class Matrix {
  std::vector<std::vector<T>> inner;

 public:
  Matrix() : inner() {}
  Matrix(size_t m, size_t n, const T &value)
      : inner(m, std::vector<T>(n, value)) {
    if (m <= 0 || n <= 0) {
      // TODO: Specify kind of expression
      throw std::exception();
    }
  }

  size_t get_height() const { return inner.size(); }
  size_t get_width() const { return inner[0].size(); }
  T &get(size_t x, size_t y) { return inner.at(y).at(x); }

  friend std::ostream &operator<<(std::ostream &os, const Matrix &mat) {
    os << '[';
    for (const auto &row : mat.inner) {
      os << '[';
      for (const auto &el : row) {
        os << el << ',';
      }
      os << "],";
    }
    os << ']';
    return os;
  }
};

}  // namespace image
