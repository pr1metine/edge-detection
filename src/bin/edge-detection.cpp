#include <iostream>
#include <systemc>

#include "convolution/Filter.h"
#include "convolution/Layer.h"
#include "image/ImageReader.h"
#include "image/ImageWriter.h"
#include "image/Matrix.h"
#include "max_pooling/Layer.h"

using namespace sc_core;

#define PIXEL_TYPE uint8_t

image::Matrix<double> getFilterFromCommandLine(std::string filter) {
  if (filter == "sobel_horizontal") {
    return convolution::sobel::create_horizontal_filter_kernel();
  }
  if (filter == "sobel_vertical") {
    return convolution::sobel::create_vertical_filter_kernel();
  }
  if (filter == "sobel_diagonal") {
    return convolution::sobel::create_diagonal_filter_kernel();
  }
  if (filter == "edge_horizontal") {
    return convolution::edge::create_horizontal_filter_kernel();
  }
  if (filter == "edge_vertical") {
    return convolution::edge::create_vertical_filter_kernel();
  }
  if (filter == "edge_diagonal") {
    return convolution::edge::create_diagonal_filter_kernel();
  }
  throw std::invalid_argument("Invalid filter argument");
}

int sc_main(int argc, char *argv[]) {
  if (argc < 3 || argc > 4) {
    std::cerr << "usage: edge-detection <filter> <path/to/input/png> "
                 "[<path/to/output/png>]\n\n"
                 "filter:\n"
                 "\tsobel_horizontal\n"
                 "\tsobel_vertical\n"
                 "\tsobel_diagonal\n"
                 "\tedge_horizontal\n"
                 "\tedge_vertical\n"
                 "\tedge_diagonal\n"
                 "\n";
    return 1;
  }

  const char *output_path = argc >= 4 ? argv[3] : "output.png";
  image::ImageReader<PIXEL_TYPE> reader("reader", argv[2]);
  convolution::Layer<PIXEL_TYPE> convolution_layer(
      "convolution_layer", reader.get_output_height(),
      reader.get_output_width(), getFilterFromCommandLine(argv[1]), 127);
  convolution::Layer<PIXEL_TYPE> blur_layer(
      "blur_layer", convolution_layer.get_output_height(),
      convolution_layer.get_output_width(),
      convolution::blur::create_blur_kernel(), 0);
  max_pooling::Layer<PIXEL_TYPE> max_pooling_layer(
      "max_pooling_layer", blur_layer.get_output_height(),
      blur_layer.get_output_width(), 2, 2, 2);
  image::ImageWriter<PIXEL_TYPE> writer("writer", output_path,
                                        max_pooling_layer.get_output_height(),
                                        max_pooling_layer.get_output_width());

  sc_fifo<image::Matrix<PIXEL_TYPE>> reader_conv;
  reader.output_matrix(reader_conv);
  convolution_layer.input_matrix(reader_conv);
  sc_fifo<image::Matrix<PIXEL_TYPE>> conv_blur;
  convolution_layer.output_matrix(conv_blur);
  blur_layer.input_matrix(conv_blur);
  sc_fifo<image::Matrix<PIXEL_TYPE>> blur_max;
  blur_layer.output_matrix(blur_max);
  max_pooling_layer.input_matrix(blur_max);
  sc_fifo<image::Matrix<PIXEL_TYPE>> max_writer;
  max_pooling_layer.output_matrix(max_writer);
  writer.input_matrix(max_writer);

  sc_start(10, SC_NS);

  std::cout << "Done!\n";

  return 0;
}
