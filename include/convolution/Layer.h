/**
 * @file Layer.h
 * @author Quang Thanh Ta (ta.quang_thanh-it21@it.dhbw-ravensburg.de)
 * @brief Header file for the convolution layer class
 * @version 0.1
 * @date 2023-12-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <exception>
#include <systemc>
#include <vector>

#include "image/Matrix.h"

namespace convolution {

/**
 * @brief Convolution Layer
 *
 * Given an input matrix of size m*n and a convolution kernel of size o*p, this
 * layer applies a convolution operation onto the input matrix. After performing
 * this convolution operation, this layer produces an output matrix of size (m -
 * o + 1) * (n - p + 1).
 *
 * In addition to the convolution operation, you may specify an offset which is
 * added onto the result of a convolution operation.
 *
 * @see
 * https://de.wikipedia.org/wiki/Convolutional_Neural_Network#Convolutional_Layer
 *
 * @note  SC_MODULE(Layer) Define macro has been manually expanded because
 * doxygen would not work otherwise
 *
 * @tparam MatrixType Element type of input and output matrix
 * @tparam KernelType Element type of kernel matrix
 */
template <typename MatrixType, typename KernelType = double>
struct Layer : ::sc_core::sc_module {
  sc_core::sc_fifo_in<image::Matrix<MatrixType>> input_matrix;
  sc_core::sc_fifo_out<image::Matrix<MatrixType>> output_matrix;
  const size_t input_height;
  const size_t input_width;
  const image::Matrix<KernelType> kernel;
  const MatrixType offset;

  SC_HAS_PROCESS(Layer);

  /**
   * @brief Construct a new Convolution Layer object
   *
   * @param name SystemC module name of newly created object
   * @param input_height Height of input matrix, i.e., amount of rows
   * @param input_width Width of input matrix, i.e., amount of columns
   * @param kernel Convolution kernel used by convolution operation
   * @param offset Offset added to all elements of convolution result matrix
   */
  Layer(sc_core::sc_module_name name, size_t input_height, size_t input_width,
        image::Matrix<KernelType> kernel, MatrixType offset)
      : sc_module(name),
        kernel(kernel),
        offset(offset),
        input_height(input_height),
        input_width(input_width) {
    if (kernel.get_height() > get_input_height()) {
      throw std::invalid_argument(
          "Kernel height is greater than input matrix height");
    }
    if (kernel.get_width() > get_input_width()) {
      throw std::invalid_argument(
          "Kernel width is greater than input matrix width");
    }
    SC_THREAD(process);
  }

  /**
   * @brief Retrieve expected height of input matrix.
   *
   * @return size_t Height of input matrix
   */
  size_t get_input_height() const { return input_height; }
  /**
   * @brief Retrieve expected width of input matrix.
   *
   * @return size_t Width of input matrix
   */
  size_t get_input_width() const { return input_width; }
  /**
   * @brief Retrieve height of output matrix.
   *
   * The height of the output matrix is calculated using the following formula:
   *
   * output_height = input_height - kernel_height + 1;
   *
   * @return size_t Height of output matrix
   */
  size_t get_output_height() const {
    return input_height - kernel.get_height() + 1;
  }
  /**
   * @brief Retrieve width of output matrix.
   *
   * The width of the output matrix is calculated using the following formula:
   *
   * output_width = input_width - kernel_width + 1;
   *
   * @return size_t Width of output matrix
   */
  size_t get_output_width() const {
    return input_width - kernel.get_width() + 1;
  }

  /**
   * @brief Performs convolution operation given an updated input matrix.
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
      const MatrixType MIN = std::numeric_limits<MatrixType>::min();
      const MatrixType MAX = std::numeric_limits<MatrixType>::max();

      for (size_t y = 0; y < get_output_height(); ++y) {
        for (size_t x = 0; x < get_output_width(); ++x) {
          KernelType curr = offset;

          for (size_t dy = 0; dy < kernel.get_height(); ++dy) {
            for (size_t dx = 0; dx < kernel.get_width(); ++dx) {
              curr += kernel.get(dx, dy) *
                      static_cast<KernelType>(in.get(x + dx, y + dy));
            }
          }

          if (curr <= static_cast<KernelType>(MIN)) {
            out.get_mut(x, y) = MIN;
            continue;
          }
          if (curr >= static_cast<KernelType>(MAX)) {
            out.get_mut(x, y) = MAX;
            continue;
          }

          out.get_mut(x, y) = static_cast<MatrixType>(curr);
        }
      }

      output_matrix.nb_write(out);
    }
  }
};

}  // namespace convolution
