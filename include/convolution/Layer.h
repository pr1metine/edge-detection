#ifndef __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_CONVOLUTIONLAYER_H_
#define __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_CONVOLUTIONLAYER_H_

#include <systemc.h>

#include <algorithm>

#include "convolution/internal/Block.h"
#include "convolution/internal/Combiner.h"
#include "convolution/internal/MatrixSelector.h"

namespace convolution {

template <unsigned int INPUT_HEIGHT, unsigned int INPUT_WIDTH,
          unsigned int OP_HEIGHT, unsigned int OP_WIDTH>
SC_MODULE(Layer) {
#define OUTPUT_HEIGHT (INPUT_HEIGHT - 2)
#define OUTPUT_WIDTH (INPUT_WIDTH - 2)

  sc_in<char[INPUT_HEIGHT][INPUT_WIDTH]> input;
  sc_in<float[OP_HEIGHT][OP_WIDTH]> op;
  sc_in<char> offset;
  sc_out<char[OUTPUT_HEIGHT][OUTPUT_WIDTH]> output;

  internal::Block<OP_HEIGHT, OP_WIDTH>* blocks[OUTPUT_HEIGHT][OUTPUT_WIDTH];
  internal::Combiner<OUTPUT_HEIGHT, OUTPUT_WIDTH> combiner;

  SC_CTOR(Layer) : combiner("combiner") {
    SC_METHOD(process);
    for (size_t y = 0; y < OUTPUT_HEIGHT; ++y) {
      for (size_t x = 0; x < OUTPUT_WIDTH; ++x) {
        char submodule_name[20];
        snprintf(submodule_name, 20, "block_%zu_%zu", x, y);
        blocks[y][x] = new internal::Block<OP_HEIGHT, OP_WIDTH>(submodule_name);
        blocks[y][x]->op(op);
        blocks[y][x]->offset(offset);
        combiner.inputs[y][x](blocks[y][x]->output);
      }
    }

    output(combiner.output);

    sensitive << input << op << offset;
  }

  void process() {
    auto in = input.read();
    char out[OUTPUT_HEIGHT][OUTPUT_WIDTH];

    for (size_t y = 0; y < OUTPUT_HEIGHT; ++y) {
      for (size_t x = 0; x < OUTPUT_WIDTH; ++x) {
        char next_input[OP_HEIGHT][OP_WIDTH] = {
            {in[y][x], in[y][x + 1], in[y][x + 2]},
            {in[y + 1][x], in[y + 1][x + 1], in[y + 1][x + 2]},
            {in[y + 2][x], in[y + 2][x + 1], in[y + 2][x + 2]},
        };
      }
    }

    output.write(out);
  }
};
}  // namespace convolution

#endif  // __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_CONVOLUTIONLAYER_H_