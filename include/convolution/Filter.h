#pragma once

#include "image/Matrix.h"

namespace convolution {

namespace sobel {

image::Matrix<double> create_horizontal_filter_kernel();
image::Matrix<double> create_vertical_filter_kernel();
image::Matrix<double> create_diagonal_filter_kernel();

}  // namespace sobel

namespace edge {

image::Matrix<double> create_horizontal_filter_kernel();
image::Matrix<double> create_vertical_filter_kernel();
image::Matrix<double> create_diagonal_filter_kernel();

}  // namespace edge

namespace blur {
image::Matrix<double> create_blur_kernel();
}  // namespace blur

}  // namespace convolution
