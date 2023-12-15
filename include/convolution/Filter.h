/**
 * @file Filter.h
 * @author Quang Thanh Ta (ta.quang_thanh-it21@it.dhbw-ravensburg.de)
 * @brief Header file for various filter kernels to be used for convolution
 *
 * @version 0.1
 * @date 2023-12-10
 *
 * @copyright Copyright (c) 2023
 *
 */
#pragma once

#include "image/Matrix.h"

namespace convolution {

namespace sobel {

/**
 * @brief Create a horizontal sobel filter kernel
 *
 * @return image::Matrix<double> Horizontal sobel, i.e., `[[1.0, 0.0, -1.0],
 * [2.0, 0.0, -2.0], [1.0, 0.0, -1.0]]`
 */
image::Matrix<double> create_horizontal_filter_kernel();

/**
 * @brief Create a vertical sobel filter kernel
 *
 * @return image::Matrix<double> Vertical sobel, i.e., `[[1.0, 2.0, 1.0], [0.0,
 * 0.0, 0.0], [-1.0, -2.0, -1.0]]`
 */
image::Matrix<double> create_vertical_filter_kernel();

/**
 * @brief Create a diagonal sobel filter kernel
 *
 * @return image::Matrix<double> Diagonal sobel, i.e., `[[0.0, 1.0, 2.0], [-1.0,
 * 0.0, 1.0], [-2.0, -1.0, 0.0]]`
 */
image::Matrix<double> create_diagonal_filter_kernel();

}  // namespace sobel

namespace edge {

/**
 * @brief Create a horizontal edge filter kernel
 *
 * @return image::Matrix<double> Horizontal edge detector, i.e., `[[-1.0, -1.0,
 * -1.0], [2.0, 2.0, 2.0], [-1.0, -1.0, -1.0]]`
 */
image::Matrix<double> create_horizontal_filter_kernel();

/**
 * @brief Create a vertical edge filter kernel
 *
 * @return image::Matrix<double> Vertical edge detector, i.e., `[[-1.0, 2.0,
 * -1.0], [-1.0, 2.0, -1.0], [-1.0, 2.0, -1.0]]`
 */
image::Matrix<double> create_vertical_filter_kernel();

/**
 * @brief Create a diagonal edge filter kernel
 *
 * @return image::Matrix<double> Diagonal edge detector, i.e., `[[-1.0,
 * -1.0, 2.0], [-1.0, 2.0, -1.0], [2.0, -1.0, -1.0]]`
 */
image::Matrix<double> create_diagonal_filter_kernel();

}  // namespace edge

namespace blur {

/**
 * @brief Create a blur kernel object
 *
 * @return image::Matrix<double> Blur kernel, i.e., `[[0.111, 0.111, 0.111],
 * [0.111, 0.111, 0.111], [0.111, 0.111, 0.111]]`
 */
image::Matrix<double> create_blur_kernel();
}  // namespace blur

}  // namespace convolution
