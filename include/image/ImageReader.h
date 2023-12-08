#pragma once

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <strstream>
#include <systemc>
#include <vector>

#include "image/Matrix.h"

namespace image {

template <typename OutputType>
SC_MODULE(ImageReader) {
  std::string path_to_png;
  boost::gil::gray8_image_t image;
  unsigned int output_height;
  unsigned int output_width;

  sc_core::sc_fifo_out<image::Matrix<OutputType>> output_matrix;

  SC_HAS_PROCESS(ImageReader);

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

  unsigned int get_output_height() const { return output_height; }
  unsigned int get_output_width() const { return output_width; }

  void process() {
    image::Matrix<OutputType> out(get_output_height(), get_output_width());
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
