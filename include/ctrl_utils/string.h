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

#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

#include "eigen_string.h"

namespace ctrl_utils {

template<typename T>
inline std::string ToString(const T& value) {
  std::stringstream ss;
  ss << value;
  return ss.str();
}

template<>
inline std::string ToString(const std::string& value) {
  return value;
}

template<>
inline std::string ToString(const nlohmann::json& value) {
  return value.dump();
}

template<typename T>
inline void ToString(std::string& str, const T& value) {
  str = ToString(value);
}

template<typename T>
inline T FromString(const std::string& str) {
  T value;
  std::stringstream ss(str);
  ss >> value;
  return value;
}

template<>
inline std::string FromString(const std::string& str) {
  return str;
}

template<>
inline nlohmann::json FromString(const std::string& str) {
  return nlohmann::json::parse(str);
}

template<>
inline YAML::Node FromString(const std::string& str) {
  return YAML::Load(str);
}

template<typename T>
inline void FromString(const std::string& str, T& value) {
  value = FromString<T>(str);
}

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_STRING_H_
