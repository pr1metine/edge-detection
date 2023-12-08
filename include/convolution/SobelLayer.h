#pragma once

#include "convolution/LayerAlt.h"

namespace convolution {
namespace sobel {

image::Matrix<double> create_horizontal_filter_kernel() {
  return image::Matrix<double>{
      {1.0, 0.0, -1.0},
      {2.0, 0.0, -2.0},
      {1.0, 0.0, -1.0},
  };
}
image::Matrix<double> create_vertical_filter_kernel() {
  return image::Matrix<double>{
      {1.0, 2.0, 1.0},
      {0.0, 0.0, 0.0},
      {-1.0, 2.0, -1.0},
  };
}
image::Matrix<double> create_diagonal_filter_kernel() {
  return image::Matrix<double>{
      {0.0, 1.0, 2.0},
      {-1.0, 0.0, 1.0},
      {-2.0, 1.0, 0.0},
  };
}

}  // namespace sobel
}  // namespace convolution
