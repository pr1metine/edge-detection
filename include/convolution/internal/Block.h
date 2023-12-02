#ifndef __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_CONVOLUTIONBLOCK_H_
#define __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_CONVOLUTIONBLOCK_H_

#include <systemc.h>

namespace convolution {
namespace internal {

template <unsigned int Height, unsigned int Width>
SC_MODULE(Block) {
  sc_in<char[Height][Width]> input;
  sc_in<float[Height][Width]> op;
  sc_in<char> offset;

  sc_out<char> output;

  SC_CTOR(Block) {
    SC_METHOD(process);
    sensitive << input;
  }

  void process() {
    float result = 0.0;
    auto in = input.read();
    auto o = op.read();

    for (int y = 0; y < Height; ++y) {
      for (int x = 0; x < Width; ++x) {
        result += in[y][x] * o[y][x];
      }
    }

    output.write((char)result + offset);
  }
};

}  // namespace internal
}  // namespace convolution

#endif  // __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_CONVOLUTIONBLOCK_H_