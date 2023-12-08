#include <iostream>
#include <systemc>

#include "convolution/LayerAlt.h"
#include "image/ImageReader.h"
#include "image/Matrix.h"
using namespace sc_core;

int sc_main(int argc, char *argv[]) {
  LayerAlt alt("layer_alt");
  //   image::ImageReader<int> reader("what", argv[1]);

  sc_fifo<image::Matrix<int>> input;
  sc_fifo<image::Matrix<int>> output;
  alt.input(input);
  alt.output(output);

  input = image::Matrix<int>(2, 2, 4);

  sc_start(10, SC_NS);

  std::cout << output << "\n";

  std::cout << "Done!\n";

  return 0;
}
