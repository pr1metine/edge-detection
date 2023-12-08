#pragma once

#include <ostream>
#include <vector>

namespace image {

template <typename T>
class Matrix {
  std::vector<std::vector<T>> inner;

 public:
  Matrix() : inner() {}
  Matrix(size_t m, size_t n, const T &value = T())
      : inner(m, std::vector<T>(n, value)) {}
  Matrix(std::initializer_list<std::vector<T>> list) : inner(list) {
    if (!is_empty()) {
      const size_t width = get_width();

      for (const std::vector<T> &row : inner) {
        if (row.size() != width) {
          throw std::invalid_argument{"Row lengths of matrix are not uniform"};
        }
      }
    }
  }

  bool is_empty() const { return inner.empty() || inner[0].empty(); }
  size_t get_height() const { return inner.size(); }
  size_t get_width() const { return inner[0].size(); }
  T &get_mut(size_t x, size_t y) { return inner.at(y).at(x); }
  T get(size_t x, size_t y) const { return inner[y][x]; }

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
