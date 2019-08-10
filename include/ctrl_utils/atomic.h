/**
 * atomic.h
 *
 * Copyright 2019. All Rights Reserved.
 *
 * Created: August 10, 2019
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_ATOMIC_H_
#define CTRL_UTILS_ATOMIC_H_

#include <mutex>    // std::mutex, std::lock_guard
#include <utility>  // std::move

namespace ctrl_utils {

template<typename T>
class Atomic {

 public:

  Atomic(const T& value) : value_(value) {}
  Atomic(T&& value) : value_(std::move(value)) {}

  Atomic(const Atomic<T>& other) : value_(other.value_) {}
  Atomic(Atomic<T>&& other) : value_(std::move(other.value_)) {}

  Atomic<T>& operator=(const T& value) {
    std::lock_guard<std::mutex> lock(mtx_);
    value_ = value;
  }
  Atomic<T>& operator=(T&& value) {
    std::lock_guard<std::mutex> lock(mtx_);
    value_ = std::move(value);
  }

  T load() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return value_;
  }

 private:

  T value_;
  mutable std::mutex mtx_;

};

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_ATOMIC_H_
