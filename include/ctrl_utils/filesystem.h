/**
 * filesystem.h
 *
 * Copyright 2019. All Rights Reserved.
 *
 * Created: January 22, 2019
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_FILESYSTEM_H_
#define CTRL_UTILS_FILESYSTEM_H_

#include <string>    // std::stringstream
#include <stdlib.h>  // realpath
#include <unistd.h>  // get_current_dir_name

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
  char* c_path = get_current_dir_name();
  path = c_path;
  free(c_path);
  return path;
}

inline std::string ParentPath(const std::string& path) {
  return path.substr(0, path.find_last_of("/\\") + 1);
}

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_FILESYSTEM_H_
