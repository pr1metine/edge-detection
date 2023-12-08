#pragma once

#include <systemc>

#include "image/Matrix.h"

namespace pipeline {

template <typename OutputType>
struct Source {
  sc_core::sc_fifo_out<image::Matrix<OutputType>> output_matrix;
  const size_t output_height;
  const size_t output_width;

  Source(size_t output_height, size_t output_width)
      : output_height(output_height), output_width(output_width) {}

  size_t get_output_height() const { return output_height; }
  size_t get_output_width() const { return output_width; }
};

}  // namespace pipeline
