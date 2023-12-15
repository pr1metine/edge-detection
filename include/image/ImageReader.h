/**
 * @file ImageReader.h
 * @author Quang Thanh Ta (ta.quang_thanh-it21@it.dhbw-ravensburg.de)
 * @brief Header file for image reader class
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <strstream>
#include <systemc>
#include <vector>

#include "image/Matrix.h"

namespace image {

/**
 * @brief Reads a PNG image using Boost GIL (Generic Image Library).
 *
 * The user must specify a path to a specific PNG image. After successfully
 * reading the image associate to that path, ImageReader writes its
 * contents once to a SystemC FIFO queue. The contents are encoded as a matrix.
 *
 * @note  SC_MODULE(Layer) Define macro has been manually expanded because
 * doxygen would not work otherwise
 *
 * @tparam MatrixType Type of elements inside the output matrix
 * @tparam ImageType Type of Boost GIL image used to decode image
 */
template <typename MatrixType, typename ImageType = boost::gil::gray8_image_t>
struct ImageReader : ::sc_core::sc_module {
  std::string path_to_png;
  ImageType image;
  size_t output_height;
  size_t output_width;

  sc_core::sc_fifo_out<image::Matrix<MatrixType>> output_matrix;

  SC_HAS_PROCESS(ImageReader);

  /**
   * @brief Construct a new Image Reader object
   *
   * @param name Name of this SystemC module name
   * @param path_to_png Path to the PNG file which is to be read.
   */
  ImageReader(sc_core::sc_module_name name, std::string path_to_png)
      : sc_module(name), path_to_png(path_to_png) {
    {
      boost::gil::read_and_convert_image(path_to_png, image,
                                         boost::gil::png_tag());
      auto v = boost::gil::view(image);
      output_height = v.dimensions().y;
      output_width = v.dimensions().x;
    }

    SC_THREAD(process);
  }

  /**
   * @brief Retrieve height of output matrix
   *
   * @return size_t Height of output matrix
   */
  size_t get_output_height() const { return output_height; }
  /**
   * @brief Retrieve width of output matrix
   *
   * @return size_t Width of output matrix
   */
  size_t get_output_width() const { return output_width; }

  /**
   * @brief Write PNG contents once to SystemC FIFO queue
   *
   */
  void process() {
    image::Matrix<MatrixType> out(get_output_height(), get_output_width());
    auto v = boost::gil::view(image);
    for (size_t y = 0; y < get_output_height(); ++y) {
      for (size_t x = 0; x < get_output_width(); ++x) {
        out.get_mut(x, y) = v(x, y);
      }
    }

    output_matrix.write(out);
  }
};

}  // namespace image
