/**
 * @file ImageWriter.h
 * @author Quang Thanh Ta (ta.quang_thanh-it21@it.dhbw-ravensburg.de)
 * @brief Header file for matrix structure
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <fstream>
#include <systemc>

#include "image/Matrix.h"

namespace image {

/**
 * @brief Saves a matrix as a PNG image file.
 *
 * The user must specify a path to which the PNG will be written to. After
 * receiving an input matrix, ImageWriter converts it to a Boost GIL image and
 * saves it to a file at the specified path. If a file at the given path already
 * exists, it will be overwritten. Otherwise, a new file will be created.
 *
 * @note  SC_MODULE(Layer) Define macro has been manually expanded because
 * doxygen would not work otherwise
 *
 * @tparam MatrixType Type of elements inside the input matrix
 * @tparam ImageType Type of Boost GIL image used to encode image
 */
template <typename MatrixType, typename ImageType = boost::gil::gray8_image_t>
struct ImageWriter : ::sc_core::sc_module {
  const std::string path_to_png;
  const size_t input_height;
  const size_t input_width;
  sc_core::sc_fifo_in<image::Matrix<MatrixType>> input_matrix;

  SC_HAS_PROCESS(ImageWriter);

  /**
   * @brief Construct a new Image Writer object
   *
   * @param name SystemC module name of Image Writer object
   * @param path_to_png Path of destination PNG file
   * @param input_height Height of input matrix
   * @param input_width Width of input matrix
   */
  ImageWriter(sc_core::sc_module_name name, std::string path_to_png,
              size_t input_height, size_t input_width)
      : sc_module(name),
        path_to_png(path_to_png),
        input_height(input_height),
        input_width(input_width) {
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
   * @brief If a new input matrix is received, turn it to a Boost GIL image and
   * save it to the specified position.
   *
   */
  void process() {
    while (true) {
      auto in = input_matrix.read();
      if (in.get_height() != get_input_height() &&
          in.get_width() != get_input_width()) {
        throw std::invalid_argument(
            "Dimensions of input matrix does not match expected input "
            "dimensions");
      }
      ImageType out(input_width, input_height);
      auto v = view(out);

      for (size_t y = 0; y < v.dimensions().y; ++y) {
        for (size_t x = 0; x < v.dimensions().x; ++x) {
          v(x, y) = in.get(x, y);
        }
      }

      std::ofstream stream(path_to_png.c_str(), std::ios::binary);
      write_view(stream, v, boost::gil::png_tag());
    }
  }
};

}  // namespace image
