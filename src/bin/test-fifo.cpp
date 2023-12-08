#include <iostream>
#include <systemc>

#include "convolution/Filter.h"
#include "convolution/Layer.h"
#include "image/ImageReader.h"
#include "image/ImageWriter.h"
#include "image/Matrix.h"
#include "max_pooling/Layer.h"

using namespace sc_core;

int sc_main(int argc, char *argv[]) {
  image::ImageReader<char> reader("what", argv[1]);
  convolution::Layer<char> convolution_layer(
      "convolution_layer", reader.get_output_height(),
      reader.get_output_width(),
      convolution::sobel::create_diagonal_filter_kernel(), 127);
  convolution::Layer<char> blur_layer(
      "blur_layer", convolution_layer.get_output_height(),
      convolution_layer.get_output_width(),
      convolution::blur::create_blur_kernel(), 0);
  max_pooling::Layer<char> max_pooling_layer(
      "max_pooling_layer", blur_layer.get_output_height(),
      blur_layer.get_output_width(), 2, 2, 2);
  image::ImageWriter<char> writer("writer", "output_alt.png",
                                  max_pooling_layer.get_output_height(),
                                  max_pooling_layer.get_output_width());

  sc_fifo<image::Matrix<char>> reader_conv;
  reader.output_matrix(reader_conv);
  convolution_layer.input_matrix(reader_conv);
  sc_fifo<image::Matrix<char>> conv_blur;
  convolution_layer.output_matrix(conv_blur);
  blur_layer.input_matrix(conv_blur);
  sc_fifo<image::Matrix<char>> blur_max;
  blur_layer.output_matrix(blur_max);
  max_pooling_layer.input_matrix(blur_max);
  sc_fifo<image::Matrix<char>> max_writer;
  max_pooling_layer.output_matrix(max_writer);
  writer.input_matrix(max_writer);

  sc_start(10, SC_NS);

  std::cout << "Done!\n";

  return 0;
}
