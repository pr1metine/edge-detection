#include <iostream>
#include <systemc>

#include "convolution/Filter.h"
#include "convolution/Layer.h"
#include "image/ImageReader.h"
#include "image/ImageWriter.h"
#include "image/Matrix.h"

using namespace sc_core;

int sc_main(int argc, char *argv[]) {
  image::ImageReader<int> reader("what", argv[1]);
  convolution::Layer<int> layer(
      "layer", reader.get_output_height(), reader.get_output_width(),
      convolution::sobel::create_diagonal_filter_kernel(), 127);
  image::ImageWriter<int> writer("writer", "output_alt.png",
                                 layer.get_output_height(),
                                 layer.get_output_width());

  sc_fifo<image::Matrix<int>> reader_layer;
  reader.output_matrix(reader_layer);
  layer.input_matrix(reader_layer);
  sc_fifo<image::Matrix<int>> layer_writer;
  layer.output_matrix(layer_writer);
  writer.input_matrix(layer_writer);

  sc_start(10, SC_NS);

  std::cout << "Done!\n";

  return 0;
}
