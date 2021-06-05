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

#include <opencv2/opencv.hpp>

#include "string.h"

namespace ctrl_utils {

/**
 * Decode a cv::Mat from a string for Redis.
 *
 * Format:
 *   "nrows ncols cvtype bytedata"
 */
template <>
inline void FromString(const std::string& str, cv::Mat& image) {
  // Read image type.
  std::stringstream ss(str);
  int type;
  ss >> type;

  switch (type) {
    case CV_8UC1:
    case CV_8UC2:
    case CV_8UC3:
    case CV_8UC4:
    case CV_16UC1:
    case CV_16UC2:
    case CV_16UC3:
    case CV_16UC4:
    case CV_32FC1:
    case CV_32FC2:
    case CV_32FC3:
    case CV_32FC4: {
      // Read buffer.
      int size;
      ss >> size;
      ss.get();  // Extract space.
      std::vector<unsigned char> buffer(size);
      ss.read(reinterpret_cast<char*>(buffer.data()), size);

      // Decode png.
      cv::imdecode(buffer, cv::IMREAD_UNCHANGED, &image);
    } break;
    default: {
      // Prepare image.
      int rows, cols;
      ss >> rows >> cols;
      image.create(rows, cols, type);

      // Read raw bytes.
      ss.read(reinterpret_cast<char*>(image.data),
              image.total() * image.elemSize());
    } break;
  }
}

/**
 * Encode a cv::Mat into a string for Redis.
 *
 * Format:
 *   "nrows ncols cvtype bytedata"
 */
inline std::string ToString(const cv::Mat& image) {
  // Write image type.
  std::stringstream ss;
  ss << image.type() << " ";

  std::vector<unsigned char> buffer;
  switch (image.type()) {
    case CV_8UC1:
    case CV_8UC2:
    case CV_8UC3:
    case CV_8UC4:
    case CV_16UC1:
    case CV_16UC2:
    case CV_16UC3:
    case CV_16UC4:
      // Encode png.
      cv::imencode(".png", image, buffer);

      // Write buffer.
      ss << buffer.size() << " ";
      ss.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
      break;
    case CV_32FC1:
    case CV_32FC2:
    case CV_32FC3:
    case CV_32FC4:
      // Encode exr.
      cv::imencode(".exr", image, buffer);

      // Write buffer.
      ss << buffer.size() << " ";
      ss.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
      break;
    default:
      // Write image dimensions.
      ss << image.rows << " " << image.cols << " ";

      // Write raw bytes.
      ss.write(reinterpret_cast<char*>(image.data),
               image.total() * image.elemSize());
      break;
  }

  return ss.str();
}

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_OPENCV_H_
