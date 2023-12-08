#pragma once

#include <systemc>
#include <vector>

#include "image/Matrix.h"

SC_MODULE(LayerAlt) {
  sc_core::sc_fifo_in<image::Matrix<int>> input;
  sc_core::sc_fifo_out<image::Matrix<int>> output;

  SC_CTOR(LayerAlt) {
    SC_METHOD(process);
    sensitive << input;
  }

  void process() {
    auto out = input.read();
    for (size_t i = 0; i < out.get_height(); ++i) {
      for (size_t j = 0; j < out.get_width(); ++j) {
        out.get(j, i) *= 2;
      }
    }
    output.write(out);
  }
};