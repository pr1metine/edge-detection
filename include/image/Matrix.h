/**
 * @file Matrix.h
 * @author Quang Thanh Ta (ta.quang_thanh-it21@it.dhbw-ravensburg.de)
 * @brief Header file of Matrix structure
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <ostream>
#include <vector>

namespace image {

/**
 * @brief Rectangular array, i.e., a matrix
 *
 * Allows for dynamic sizing which is useful during SystemC elaboration.
 *
 * @tparam T Element type of matrix
 */
template <typename T>
class Matrix {
  std::vector<std::vector<T>> inner;

 public:
  /**
   * @brief Construct an empty Matrix object
   *
   */
  Matrix() : inner() {}

  /**
   * @brief Construct a new Matrix object with a given height, width, and an
   * optional default value.
   *
   * @param m Height of new matrix
   * @param n Width of new matrix
   * @param value Default value (defaults to T())
   */
  Matrix(size_t m, size_t n, const T &value = T())
      : inner(m, std::vector<T>(n, value)) {}

  /**
   * @brief Convenience constructor for constructing a new Matrix object with an
   * initializer_list just like an std::vector.
   *
   * @throw std::invalid_argument All rows must have the same length
   *
   * @param list Initializer list, e.g., {{1},{2}}
   */
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

  /**
   * @brief Checks if matrix does not contain any elements
   *
   * @return true Matrix is empty
   * @return false Matrix does contain elements
   */
  bool is_empty() const { return inner.empty() || inner[0].empty(); }
  /**
   * @brief Retrieve height of current matrix
   *
   * @return size_t Height of matrix
   */
  size_t get_height() const { return inner.size(); }
  /**
   * @brief Retrieve width of current matrix
   *
   * @return size_t Width of matrix
   */
  size_t get_width() const { return is_empty() ? 0 : inner[0].size(); }
  /**
   * @brief Get a mutable reference of a specific element in an array
   *
   * @param x Column containing the desired element (Zero-indexed)
   * @param y Row containing the desired element (Zero-indexed)
   * @return T& Reference to the desired element
   */
  T &get_mut(size_t x, size_t y) { return inner.at(y).at(x); }
  /**
   * @brief Get a copy of a specific element in an array
   *
   * @param x Column containing the desired element (Zero-indexed)
   * @param y Row containing the desired element (Zero-indexed)
   * @return T Copy of desired element
   */
  T get(size_t x, size_t y) const { return inner.at(y).at(x); }

  /**
   * @brief Serializes a matrix to an output stream
   *
   * As of yet, it assumes that ostream is std::cout. It, therefore, returns a
   * user-friendly representation of the matrix.
   *
   * @param os Output stream
   * @param mat The matrix to be serialized
   * @return std::ostream& Reference to output stream for chaining
   * serializations
   */
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
