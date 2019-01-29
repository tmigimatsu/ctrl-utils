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

namespace ctrl_utils {

template<typename T>
void ToString(std::string& str, const T& value) {
  std::stringstream ss;
  ss << value;
  str = ss.str();
}

template<>
inline void ToString(std::string& str, const std::string& value) {
  str = value;
}

template<>
inline void ToString(std::string& str, const nlohmann::json& value) {
  str = value.dump();
}

template<typename T>
void FromString(const std::string& str, T& value) {
  std::stringstream ss(str);
  ss >> value;
}

template<>
inline void FromString(const std::string& str, std::string& value) {
  value = str;
}

template<>
inline void FromString(const std::string& str, nlohmann::json& value) {
  value = nlohmann::json::parse(str);
}

template<>
inline void FromString(const std::string& str, YAML::Node& value) {
  value = YAML::Load(str);
}

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_STRING_H_
