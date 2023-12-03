#include <iostream>
#include <systemc>

#include "convolution/Layer.h"

using namespace sc_core;

int sc_main(int argc, char *argv[]) {
  convolution::Layer<char> layer(
      "layer", 3, 3, convolution::ConvolutionType::SOBER_VERTICAL, 127);

  sc_vector<sc_signal<char>> input;
  input.init(9);

  for (size_t i = 0; i < input.size(); ++i) {
    layer.input_matrix(input[i]);
    input[i] = i;
  }

  sc_signal<char> output;
  layer.output_matrix(output);

  sc_start(10, SC_NS);

  std::cout << "Done! " << ((int)output.read()) << "\n";
  return 0;
}