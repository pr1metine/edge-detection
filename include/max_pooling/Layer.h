#pragma once

#include <systemc>
#include <tuple>

namespace max_pooling {

template <typename InputType>
// SC_MODULE(Layer), public pipeline::Pass<InputType> {
SC_MODULE(Layer) {
  sc_core::sc_fifo_in<image::Matrix<InputType>> input_matrix;
  sc_core::sc_fifo_out<image::Matrix<InputType>> output_matrix;
  const size_t input_height;
  const size_t input_width;
  const size_t pooling_height;
  const size_t pooling_width;
  const size_t stride_length;

  SC_HAS_PROCESS(Layer);

  Layer(sc_core::sc_module_name name, size_t input_height, size_t input_width,
        size_t pooling_height, size_t pooling_width, size_t stride_length)
      : sc_module(name),
        input_height(input_height),
        input_width(input_width),
        pooling_height(pooling_height),
        pooling_width(pooling_width),
        stride_length(stride_length) {
    if (pooling_height > input_height || pooling_width > input_width) {
      throw std::invalid_argument(
          "Invalid pooling dimensions with respective to input dimensions");
    }

    SC_THREAD(process);
  }

  size_t get_input_height() const { return input_height; }
  size_t get_input_width() const { return input_width; }
  size_t get_output_height() const {
    const size_t count = input_height - pooling_height + 1;
    return count / stride_length + (count % stride_length != 0 ? 1 : 0);
  }
  size_t get_output_width() const {
    const size_t count = input_width - pooling_width + 1;
    return count / stride_length + (count % stride_length != 0 ? 1 : 0);
  }

  void process() {
    while (true) {
      image::Matrix<InputType> in = input_matrix.read();

      image::Matrix<InputType> out(get_output_height(), get_output_width());

      for (size_t y = 0; y < get_output_height(); ++y) {
        for (size_t x = 0; x < get_output_width(); ++x) {
          InputType curr = std::numeric_limits<InputType>::min();
          for (size_t dy = 0; dy < pooling_height; ++dy) {
            for (size_t dx = 0; dx < pooling_width; ++dx) {
              curr = std::max(curr, in.get(x * pooling_width + dx,
                                           y * pooling_height + dy));
            }
          }
          out.get_mut(x, y) = curr;
        }
      }

      output_matrix.nb_write(out);
    }
  }
};

}  // namespace max_pooling
