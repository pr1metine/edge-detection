#pragma once

#include <boost/gil.hpp>
#include <boost/gil/extension/io/png.hpp>
#include <fstream>
#include <systemc>

template <typename Type>
SC_MODULE(ImageWriter) {
  const std::string path_to_png;
  const unsigned int input_height;
  const unsigned int input_width;
  const std::vector<std::vector<Type>> output_matrix;
  sc_core::sc_port<sc_core::sc_signal_in_if<Type>, 0> input_matrix;

  SC_HAS_PROCESS(ImageWriter);

  ImageWriter(sc_core::sc_module_name name, std::string path_to_png,
              unsigned int input_height, unsigned int input_width)
      : sc_module(name),
        path_to_png(path_to_png),
        input_height(input_height),
        input_width(input_width),
        output_matrix(input_height, std::vector<Type>(input_width)) {
    SC_METHOD(process);
    sensitive << input_matrix;
  }

  void process() {
    using namespace boost::gil;

    gray8_image_t out(input_width, input_height);
    auto v = view(out);

    for (size_t y = 0; y < v.dimensions().y; ++y) {
      for (size_t x = 0; x < v.dimensions().x; ++x) {
        v(x, y) = input_matrix[y * input_width + x]->read();
      }
    }

    std::ofstream stream(path_to_png.c_str(), std::ios::binary);
    write_view(stream, v, png_tag());

    std::cout << "Triggered!\n";
  }
};