/**
 * @file Layer.h
 * @author Quang Thanh Ta (ta.quang_thanh-it21@it.dhbw-ravensburg.de)
 * @brief Header file of the max pooling layer class
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <systemc>
#include <tuple>

namespace max_pooling {

/**
 * @brief Max Pooling Layer
 *
 * Given an input matrix, this max pooling layer will reduce its dimensions by
 * taking max values from certain sections.
 *
 * The matrix is dissected into rectangular pooling areas which are separated by
 * a specific stride length.
 *
 * @see
 * https://en.wikipedia.org/wiki/Convolutional_neural_network#Pooling_layers
 *
 * @tparam MatrixType Element type of input and output matrix
 */
template <typename MatrixType>
struct Layer : ::sc_core::sc_module {
  sc_core::sc_fifo_in<image::Matrix<MatrixType>> input_matrix;
  sc_core::sc_fifo_out<image::Matrix<MatrixType>> output_matrix;
  const size_t input_height;
  const size_t input_width;
  const size_t pooling_height;
  const size_t pooling_width;
  const size_t stride_length;

  SC_HAS_PROCESS(Layer);

  /**
   * @brief Construct a new Max Pooling Layer object
   *
   * @param name SystemC module name of Max Pooling Layer object
   * @param input_height Height of input matrix
   * @param input_width Width of input matrix
   * @param pooling_height Height of pooling area
   * @param pooling_width Width of pooling area
   * @param stride_length Stride length between two adjacent pooling area, i.e.,
   * x_pool_2 - x_pool_1 == stride_length or y_pool_2 - y_pool_1 ==
   * stride_length
   */
  Layer(sc_core::sc_module_name name, size_t input_height, size_t input_width,
        size_t pooling_height, size_t pooling_width, size_t stride_length)
      : sc_module(name),
        input_height(input_height),
        input_width(input_width),
        pooling_height(pooling_height),
        pooling_width(pooling_width),
        stride_length(stride_length) {
    if (pooling_height > input_height || pooling_width > input_width) {
      throw std::invalid_argument(
          "Invalid pooling dimensions with respective to input dimensions");
    }

    SC_THREAD(process);
  }

  /**
   * @brief Retrieve expected height of input matrix
   *
   * @return size_t Height of input matrix
   */
  size_t get_input_height() const { return input_height; }
  /**
   * @brief Retrieve expected width of input matrix
   *
   * @return size_t Width of input matrix
   */
  size_t get_input_width() const { return input_width; }
  /**
   * @brief Retrieve height of output matrix
   *
   * @return size_t Height of output matrix
   */
  size_t get_output_height() const {
    const size_t count = input_height - pooling_height + 1;
    return count / stride_length + (count % stride_length != 0 ? 1 : 0);
  }
  /**
   * @brief Retrieve width of output matrix
   *
   * @return size_t Width of output matrix
   */
  size_t get_output_width() const {
    const size_t count = input_width - pooling_width + 1;
    return count / stride_length + (count % stride_length != 0 ? 1 : 0);
  }

  /**
   * @brief Perform max pooling on input matrix and store results in output
   * matrix.
   *
   */
  void process() {
    while (true) {
      image::Matrix<MatrixType> in = input_matrix.read();
      if (in.get_height() != get_input_height() &&
          in.get_width() != get_input_width()) {
        throw std::invalid_argument(
            "Dimensions of input matrix does not match expected input "
            "dimensions");
      }

      image::Matrix<MatrixType> out(get_output_height(), get_output_width());

      for (size_t y = 0; y < get_output_height(); ++y) {
        for (size_t x = 0; x < get_output_width(); ++x) {
          MatrixType curr = std::numeric_limits<MatrixType>::min();
          for (size_t dy = 0; dy < pooling_height; ++dy) {
            for (size_t dx = 0; dx < pooling_width; ++dx) {
              curr = std::max(
                  curr, in.get(x * stride_length + dx, y * stride_length + dy));
            }
          }
          out.get_mut(x, y) = curr;
        }
      }

      output_matrix.nb_write(out);
    }
  }
};

}  // namespace max_pooling
