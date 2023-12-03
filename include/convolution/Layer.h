#pragma once

#include <iostream>

namespace convolution {

enum class ConvolutionType : size_t {
  SOBER_HORIZONTAL = 0,
  SOBER_VERTICAL,
  SOBER_DIAGONAL,
  LINE_HORIZONTAL,
  LINE_VERTICAL,
  LINE_DIAGONAL,
  BLUR,
};

static float kernels[7][3][3] = {
    {
        {1.0, 0.0, -1.0},
        {2.0, 0.0, -2.0},
        {1.0, 0.0, -1.0},
    },
    {
        {1.0, 2.0, 1.0},
        {0.0, 0.0, 0.0},
        {-1.0, 2.0, -1.0},
    },
    {
        {0.0, 1.0, 2.0},
        {-1.0, 0.0, 1.0},
        {-2.0, 1.0, 0.0},
    },
    {
        {-1.0, -1.0, -1.0},
        {2.0, 2.0, 2.0},
        {-1.0, -1.0, -1.0},
    },
    {
        {-1.0, 2.0, -1.0},
        {-1.0, 2.0, -1.0},
        {-1.0, 2.0, -1.0},
    },
    {
        {-1.0, -1.0, 2.0},
        {-1.0, 2.0, -1.0},
        {2.0, -1.0, -1.0},
    },
    {
        {-0.111, -0.111, -0.111},
        {-0.111, -0.111, -0.111},
        {-0.111, -0.111, -0.111},
    },
};

template <typename Type>
SC_MODULE(Layer) {
  const unsigned int input_height;
  const unsigned int input_width;
  const ConvolutionType convolution_type;

  sc_core::sc_port<sc_core::sc_signal_in_if<Type>, 0> input_matrix;
  sc_core::sc_port<sc_core::sc_signal_out_if<Type>, 0> output_matrix;

  SC_HAS_PROCESS(Layer);

  Layer(sc_core::sc_module_name name, unsigned int input_height,
        unsigned int input_width, ConvolutionType convolution_type)
      : sc_module(name),
        input_height(input_height),
        input_width(input_width),
        convolution_type(convolution_type) {
    SC_METHOD(process);
    sensitive << input_matrix;
  }

  sc_core::sc_signal_in_if<Type> *get_input_pixel(size_t x, size_t y) {
    return input_matrix[y * get_input_width() + x];
  }
  sc_core::sc_signal_out_if<Type> *get_output_pixel(size_t x, size_t y) {
    return output_matrix[y * get_output_width() + x];
  }
  unsigned int get_input_height() const { return input_height; }
  unsigned int get_input_width() const { return input_width; }
  unsigned int get_output_height() const { return input_height - 2; }
  unsigned int get_output_width() const { return input_width - 2; }

  void process() {
    for (size_t curr_y = 1; curr_y < input_height - 1; ++curr_y) {
      for (size_t curr_x = 1; curr_x < input_width - 1; ++curr_x) {
        float curr = 0;
        for (size_t i = 0; i < 3; ++i) {
          for (size_t j = 0; j < 3; ++j) {
            curr += kernels[static_cast<size_t>(convolution_type)][i][j] *
                    get_input_pixel(curr_x - 1 + j, curr_y - 1 + i)->read();
          }
        }
        get_output_pixel(curr_x - 1, curr_y - 1)
            ->write(static_cast<Type>(curr));
      }
    }
  }
};

}  // namespace convolution
