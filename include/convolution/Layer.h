#pragma once

#include <systemc>
#include <vector>

#include "image/Matrix.h"

namespace convolution {

template <typename InputType, typename KernelType = double>
SC_MODULE(Layer) {
  sc_core::sc_fifo_in<image::Matrix<InputType>> input_matrix;
  sc_core::sc_fifo_out<image::Matrix<InputType>> output_matrix;
  const size_t input_height;
  const size_t input_width;
  const image::Matrix<KernelType> kernel;
  const InputType offset;

  SC_HAS_PROCESS(Layer);

  Layer(sc_core::sc_module_name name, size_t input_height, size_t input_width,
        image::Matrix<KernelType> kernel, InputType offset)
      : kernel(kernel),
        offset(offset),
        input_height(input_height),
        input_width(input_width) {
    assert(kernel.get_height() <= get_input_height());
    assert(kernel.get_width() <= get_input_width());
    SC_THREAD(process);
  }

  size_t get_input_height() const { return input_height; }
  size_t get_input_width() const { return input_width; }
  size_t get_output_height() const {
    return input_height - kernel.get_height() + 1;
  }
  size_t get_output_width() const {
    return input_width - kernel.get_width() + 1;
  }

  void process() {
    while (true) {
      image::Matrix<InputType> in = input_matrix.read();
      assert(in.get_height() == get_input_height());
      assert(in.get_width() == get_input_width());

      image::Matrix<InputType> out(get_output_height(), get_output_width());
      const InputType MIN = std::numeric_limits<InputType>::min();
      const InputType MAX = std::numeric_limits<InputType>::max();

      for (size_t y = 0; y < get_output_height(); ++y) {
        for (size_t x = 0; x < get_output_width(); ++x) {
          KernelType curr = offset;

          for (size_t dx = 0; dx < kernel.get_width(); ++dx) {
            for (size_t dy = 0; dy < kernel.get_height(); ++dy) {
              curr += kernel.get(dx, dy) * in.get(x + dx, y + dy);
            }
          }

          if (curr <= MIN) {
            out.get_mut(x, y) = MIN;
            continue;
          }
          if (curr >= MAX) {
            out.get_mut(x, y) = MAX;
            continue;
          }

          out.get_mut(x, y) = static_cast<InputType>(curr);
        }
      }

      output_matrix.nb_write(out);
    }
  }
};

}  // namespace convolution
