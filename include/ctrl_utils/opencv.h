/**
 * opencv.h
 *
 * Copyright 2019. All Rights Reserved.
 *
 * Created: May 9, 2019
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_OPENCV_H_
#define CTRL_UTILS_OPENCV_H_

#include <sstream>  // std::stringstream

#include <opencv2/opencv.hpp>

namespace cv {

/**
 * Decode a cv::Mat from a string for Redis.
 *
 * Format:
 *   "nrows ncols cvtype bytedata"
 */
std::stringstream& operator>>(std::stringstream& ss, Mat& image) {
  // Read image dimensions
  int rows, cols, type;
  ss >> rows >> cols >> type;
  assert(ss.get() == ' ');

  // Resize output image if necessary
  image.create(rows, cols, type);

  // Read raw image data
  const size_t num_bytes = image.total() * image.elemSize();
  ss.read(reinterpret_cast<char*>(image.data), num_bytes);

  return ss;
}

/**
 * Encode a cv::Mat into a string for Redis.
 *
 * Format:
 *   "nrows ncols cvtype bytedata"
 */
std::stringstream& operator<<(std::stringstream& ss, const Mat& image) {
  // Write image dimensions
  ss << image.rows << " " << image.cols << " " << image.type() << " ";

  // Write raw image data
  const size_t num_bytes = image.total() * image.elemSize();
  ss.write(reinterpret_cast<char*>(image.data), num_bytes);

  return ss;
}

}  // namespace cv

#endif  // CTRL_UTILS_OPENCV_H_
