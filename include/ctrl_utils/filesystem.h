/**
 * filesystem.h
 *
 * Copyright 2019. All Rights Reserved.
 *
 * Created: January 22, 2019
 * Authors: Toki Migimatsu
 */

#if __cplusplus >= 201703L
#include <filesystem>  // std::filesystem
#else   // __cplusplus

#ifndef CTRL_UTILS_FILESYSTEM_H_
#define CTRL_UTILS_FILESYSTEM_H_

#include <string>    // std::stringstream
#include <stdlib.h>  // realpath
#include <unistd.h>  // access, getcwd

namespace std {
namespace filesystem {

class path {

 public:

  path(const std::string& source) : str_(source) {}

  const char* c_str() const { return str_.c_str(); }
  const std::string& string() const { return str_; }

 private:

  std::string str_;

};

inline bool exists(const path& p) {
  return access(p.c_str(), F_OK) == 0;
}

}  // namespace filesystem
}  // namespace std

namespace ctrl_utils {

inline std::string AbsolutePath(const std::string& path) {
  std::string abs_path;
  char* c_path = realpath(path.c_str(), NULL);
  abs_path = c_path;
  free(c_path);
  return abs_path;
}

inline std::string CurrentPath() {
  std::string path;
  char* c_path = getcwd(NULL, 0);
  path = c_path;
  free(c_path);
  return path;
}

inline std::string ParentPath(const std::string& path) {
  return path.substr(0, path.find_last_of("/\\") + 1);
}

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_FILESYSTEM_H_

#endif  // __cplusplus
