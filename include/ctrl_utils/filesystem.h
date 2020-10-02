/**
 * filesystem.h
 *
 * Copyright 2019. All Rights Reserved.
 *
 * Created: January 22, 2019
 * Authors: Toki Migimatsu
 */

#if __cplusplus >= 201703L && (!__GNUG__ || __GNUC__ > 7)
#include <filesystem>  // std::filesystem
#else   // __cplusplus

#ifndef CTRL_UTILS_FILESYSTEM_H_
#define CTRL_UTILS_FILESYSTEM_H_

#include <exception>  // std::system_error
#include <memory>     // std::shared_ptr
#include <regex>      // std::regex, std::regex_replace
#include <string>     // std::stringstream
#include <iostream>   // std::basic_ostream

#include <dirent.h>   // DIR, dirent, opendir, readdir, closedir
#include <errno.h>    // errno
#include <stdlib.h>   // realpath
#include <string.h>   // strcmp
#include <unistd.h>   // access, getcwd

namespace std {
namespace filesystem {

class filesystem_error : public system_error {
};

class path {

 public:

  path() = default;

  path(const char* source) : str_(normalize(source)) {}

  path(const std::string& source) : str_(normalize(source)) {}

  const char* c_str() const { return str_.c_str(); }

  const std::string& string() const { return str_; }

  bool empty() const { return str_.empty(); }

  path parent_path() const { return str_.substr(0, str_.find_last_of("/\\") + 1); };

  path stem() const {
    const size_t idx_start = str_.find_last_of("/\\") + 1;
    const size_t idx_end = str_.find_last_of(".");
    if (idx_start == idx_end) {
      return str_.substr(idx_start);
    } else {
      return str_.substr(idx_start, idx_end - idx_start);
    }
  }

  path filename() const {
    const size_t idx_start = str_.find_last_of("/\\") + 1;
    return str_.substr(idx_start);
  }

  bool operator==(const path& other) const { return str_ == other.str_; }

 private:

  static std::string normalize(const std::string& source) {
    std::string result = std::regex_replace(source, std::regex("/+"), "/");
    if (!result.empty() && result.back() == '.') {
      result += "/";  // Append / after . for simpler regex
    }
    result = std::regex_replace(result, std::regex("/\\./"), "/");
    result = std::regex_replace(result, std::regex("^\\./"), "");
    std::string result_new = std::regex_replace(result, std::regex("[^/\\.]+/\\.\\./"), "");
    while (result_new != result) {
      result = result_new;
      result_new = std::regex_replace(result, std::regex("[^/\\.]+/\\.\\./"), "");
    }
    result = std::regex_replace(result, std::regex("^/\\.\\."), "/");
    result = std::regex_replace(result, std::regex("^\\.\\./$"), "..");
    if (result.empty()) {
      result = ".";
    }
    return result;
  }

  std::string str_;

};

inline path operator/(const path& a, const path& b) {
  return a.string() + "/" + b.string();
}

template<class CharT, class Traits>
basic_ostream<CharT, Traits>& operator<<(basic_ostream<CharT, Traits>& os, const path& p) {
  os << p.string();
}

class directory_entry {

 public:

  directory_entry() = default;

  directory_entry(const path& p) : path_(p) {}

  const filesystem::path& path() const { return path_; }

  void assign(const filesystem::path& p) { path_ = p; }

  bool operator==(const directory_entry& other) const { return path_ == other.path_; }

 private:

  filesystem::path path_;

};

class directory_iterator {

 public:

  using value_type = directory_entry;
  using difference_type = ptrdiff_t;
  using pointer = const value_type*;
  using reference = const value_type&;
  using iterator_category = std::input_iterator_tag;

  directory_iterator() = default;

  directory_iterator(const directory_iterator& other)
      : path_(other.path_), entry_(other.entry_), dp_(other.dp_), dirp_(other.dirp_) {}

  directory_iterator(const path& p)
      : path_(p) {
    dp_ = std::shared_ptr<DIR>(opendir(path_.c_str()), closedir);
    if (!dp_) {
      throw runtime_error("directory_iterator: invalid path " + path_.string());
    }
    dirp_ = readdir(dp_.get());
    while ((dirp_ != nullptr &&
           strcmp(dirp_->d_name, ".") == 0) || strcmp(dirp_->d_name, "..") == 0) {
      dirp_ = readdir(dp_.get());
    }
    if (dirp_ != nullptr) {
      entry_.assign(path(path_.string() + "/" + std::string(dirp_->d_name)));
    }
  }

  directory_iterator& operator++() {
    dirp_ = readdir(dp_.get());
    entry_ = dirp_ == nullptr ? path() : path(path_.string() + "/" + std::string(dirp_->d_name));
    return *this;
  }

  bool operator==(const directory_iterator& other) const {
    if (entry_.path().empty() && other.entry_.path().empty()) return true;
    return path_ == other.path_ && entry_ == other.entry_;
  }
  bool operator!=(const directory_iterator& other) const { return !(*this == other); }

  reference operator*() const { return entry_; }
  pointer operator->() const { return &entry_; }

 private:

  path path_;
  directory_entry entry_;

  std::shared_ptr<DIR> dp_;
  struct dirent* dirp_ = nullptr;

};

inline directory_iterator begin(directory_iterator it) {
  return it;
}

inline directory_iterator end(const directory_iterator& it) {
  return directory_iterator();
}

inline bool exists(const path& p) {
  return access(p.c_str(), F_OK) == 0;
}

inline path current_path() {
  char* c_path = getcwd(nullptr, 0);
  path p(c_path);
  free(c_path);
  return p;
}

inline path absolute_path(const path& p) {
  char* c_path = realpath(p.c_str(), nullptr);
  path p_abs(c_path);
  free(c_path);
  return p_abs;
}

}  // namespace filesystem
}  // namespace std

#endif  // CTRL_UTILS_FILESYSTEM_H_

#endif  // __cplusplus
