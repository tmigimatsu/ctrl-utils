/**
 * string.h
 *
 * Copyright 2019. All Rights Reserved.
 *
 * Created: January 22, 2019
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_STRING_H_
#define CTRL_UTILS_STRING_H_

#include <sstream>  // std::string
#include <string>   // std::stringstream

namespace ctrl_utils {

/**
 * Converts the value to a string using stringstream.
 */
template <typename T>
inline std::string ToString(const T& value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

/**
 * Template specialization for strings.
 */
template <>
inline std::string ToString(const std::string& value) {
  return value;
}

/**
 * Converts the string to a value using stringstream.
 */
template <typename T>
inline void FromString(const std::string& str, T& value) {
  std::stringstream ss(str);
  ss >> value;
}

/**
 * Converts the string to a value using stringstream.
 */
template <typename T>
inline T FromString(const std::string& str) {
  T value;
  FromString(str, value);
  return value;
}

/**
 * Template specialization for strings.
 */
template <>
inline void FromString(const std::string& str, std::string& value) {
  value = str;
}

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_STRING_H_
