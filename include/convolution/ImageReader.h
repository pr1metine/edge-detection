#pragma once

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <strstream>
#include <systemc>
#include <vector>

namespace convolution {

template <typename OutputType>
SC_MODULE(ImageReader) {
  std::string path_to_png;
  boost::gil::gray8_image_t image;
  unsigned int output_height;
  unsigned int output_width;

  sc_core::sc_port<sc_core::sc_signal_out_if<OutputType>, 0> output_matrix;

  SC_HAS_PROCESS(ImageReader);

  ImageReader(sc_core::sc_module_name name, std::string path_to_png)
      : sc_module(name), path_to_png(path_to_png) {
    {
      using namespace boost::gil;
      read_and_convert_image(path_to_png, image, png_tag());
      auto v = view(image);
      output_height = v.dimensions().y;
      output_width = v.dimensions().x;
    }

    SC_THREAD(process);
  }

  sc_core::sc_signal_out_if<OutputType> *get_pixel(size_t x, size_t y) {
    return output_matrix[y * get_output_width() + x];
  }
  unsigned int get_output_height() const { return output_height; }
  unsigned int get_output_width() const { return output_width; }

  void process() {
    auto v = view(image);
    for (size_t y = 0; y < get_output_height(); ++y) {
      for (size_t x = 0; x < get_output_width(); ++x) {
        get_pixel(x, y)->write(static_cast<OutputType>(v(x, y)));
      }
    }
  }
};

}  // namespace convolution
