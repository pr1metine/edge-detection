#ifndef __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_CONVOLUTIONMATRIXSELECTOR_H_
#define __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_CONVOLUTIONMATRIXSELECTOR_H_

#include <systemc.h>

namespace convolution {
namespace internal {

template <unsigned int INPUT_HEIGHT, unsigned int INPUT_WIDTH,
          unsigned int Y_START, unsigned int Y_END, unsigned int X_START,
          unsigned int X_END>
SC_MODULE(MatrixSelector) {
  sc_in<char[INPUT_HEIGHT][INPUT_WIDTH]> input;
  sc_out<char[Y_END - Y_START][X_END - X_START]> output;

  SC_CTOR(MatrixSelector) {
    SC_METHOD(process);
    sensitive << input;
  }
  void process() {
    char out[Y_END - Y_START][X_END - X_START];
    auto in = input.read();

    for (size_t y = Y_START; y < Y_END; ++y) {
      for (size_t x = X_START; x < X_START; ++x) {
        out[y - Y_START][x - X_START] = in[y][x];
      }
    }

    output.write(out);
  }
};

}  // namespace internal
}  // namespace convolution

#endif  // __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_CONVOLUTIONMATRIXSELECTOR_H_