#pragma once

#include <systemc>

#include "image/Matrix.h"

namespace pipeline {

template <typename InputType>
struct Sink {
  sc_core::sc_fifo_in<image::Matrix<InputType>> input_matrix;
  const size_t input_height;
  const size_t input_width;

  Sink(size_t input_height, size_t input_width)
      : input_height(input_height), input_width(input_width) {}

  size_t get_input_height() { return input_height; }
  size_t get_input_width() { return input_width; }
};

}  // namespace pipeline
