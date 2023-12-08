#pragma once

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <fstream>
#include <systemc>

#include "image/Matrix.h"

namespace image {
template <typename InputType, typename ImageType = boost::gil::gray8_image_t>
SC_MODULE(ImageWriter) {
  const std::string path_to_png;
  const unsigned int input_height;
  const unsigned int input_width;
  sc_core::sc_fifo_in<image::Matrix<InputType>> input_matrix;

  SC_HAS_PROCESS(ImageWriter);

  ImageWriter(sc_core::sc_module_name name, std::string path_to_png,
              unsigned int input_height, unsigned int input_width)
      : sc_module(name),
        path_to_png(path_to_png),
        input_height(input_height),
        input_width(input_width) {
    SC_THREAD(process);
  }

  void process() {
    while (true) {
      auto input = input_matrix.read();
      ImageType out(input_width, input_height);
      auto v = view(out);

      for (size_t y = 0; y < v.dimensions().y; ++y) {
        for (size_t x = 0; x < v.dimensions().x; ++x) {
          v(x, y) = input.get(x, y);
        }
      }

      std::ofstream stream(path_to_png.c_str(), std::ios::binary);
      write_view(stream, v, boost::gil::png_tag());
    }
  }
};

}  // namespace image
