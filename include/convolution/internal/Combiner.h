#ifndef __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_CONVOLUTIONCOMBINER_H_
#define __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_CONVOLUTIONCOMBINER_H_

#include <systemc.h>

namespace convolution {
namespace internal {

template <unsigned int HEIGHT, unsigned int WIDTH>
SC_MODULE(Combiner) {
  sc_in<char> inputs[HEIGHT][WIDTH];
  sc_out<char[HEIGHT][WIDTH]> output;

  SC_CTOR(Combiner) {
    SC_METHOD(process);
    for (size_t y = 0; y < HEIGHT; ++y) {
      for (size_t x = 0; x < WIDTH; ++x) {
        sensitive << inputs[y][x];
      }
    }
  }

  void process() {
    char out[HEIGHT][WIDTH];
    for (size_t y = 0; y < HEIGHT; ++y) {
      for (size_t x = 0; x < WIDTH; ++x) {
        out[y][x] = inputs[y][x].read();
      }
    }
    output.write(out);
  }
};

}  // namespace internal
}  // namespace convolution

#endif  // __PROJECTS_SYSTEMC_EDGE_DETECTION_INCLUDE_CONVOLUTIONCOMBINER_H_