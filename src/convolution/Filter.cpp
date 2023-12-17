/**
 * @file Filter.cpp
 * @author Quang Thanh Ta (ta.quang_thanh-it21@it.dhbw-ravensburg.de)
 * @brief Source file containing function implementations for creating filter
 * kernels
 * @version 0.1
 * @date 2023-12-15
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "convolution/Filter.h"

namespace convolution {
namespace sobel {

image::Matrix<double> create_horizontal_filter_kernel() {
  return image::Matrix<double>{
      {1.0, 2.0, 1.0},
      {0.0, 0.0, 0.0},
      {-1.0, -2.0, -1.0},
  };
}
image::Matrix<double> create_vertical_filter_kernel() {
  return image::Matrix<double>{
      {1.0, 0.0, -1.0},
      {2.0, 0.0, -2.0},
      {1.0, 0.0, -1.0},
  };
}
image::Matrix<double> create_diagonal_filter_kernel() {
  return image::Matrix<double>{
      {0.0, 1.0, 2.0},
      {-1.0, 0.0, 1.0},
      {-2.0, -1.0, 0.0},
  };
}

}  // namespace sobel

namespace edge {

image::Matrix<double> create_horizontal_filter_kernel() {
  return image::Matrix<double>{
      {-1.0, -1.0, -1.0},
      {2.0, 2.0, 2.0},
      {-1.0, -1.0, -1.0},
  };
}

image::Matrix<double> create_vertical_filter_kernel() {
  return image::Matrix<double>{
      {-1.0, 2.0, -1.0},
      {-1.0, 2.0, -1.0},
      {-1.0, 2.0, -1.0},
  };
}

image::Matrix<double> create_diagonal_filter_kernel() {
  return image::Matrix<double>{
      {-1.0, -1.0, 2.0},
      {-1.0, 2.0, -1.0},
      {2.0, -1.0, -1.0},
  };
}

}  // namespace edge

namespace blur {

image::Matrix<double> create_blur_kernel() {
  return image::Matrix<double>{
      {0.111, 0.111, 0.111},
      {0.111, 0.111, 0.111},
      {0.111, 0.111, 0.111},
  };
}
}  // namespace blur

}  // namespace convolution
