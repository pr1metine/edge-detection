#include <iostream>
#include <vector>

#include "convolution/ImageReader.h"

using namespace sc_core;

int sc_main(int argc, char *argv[]) {
  convolution::ImageReader<char> reader("reader", argv[1]);

  sc_core::sc_vector<sc_core::sc_signal<char>> output;
  output.init(reader.get_output_width() * reader.get_output_height());

  for (size_t i = 0; i < output.size(); ++i) {
    reader.output_matrix(output[i]);
  }
  //   sc_signal<char> help[100];

  //   sc_fifo<char[1000][1000]> output;
  //   reader.output_matrix(output);

  sc_start(10, SC_NS);

  std::cout << "Done!\n";
  return 0;
}