/**
 * semaphore.h
 *
 * Copyright 2021. All Rights Reserved.
 *
 * Created: May 21, 2021
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_SEMAPHORE_H_
#define CTRL_UTILS_SEMAPHORE_H_

#include <condition_variable>  // std::condition_variable
#include <mutex>               // std::mutex, std::unique_lock

namespace std {

class binary_semaphore {
 public:
  explicit binary_semaphore(bool desired) : val_(desired) {}

  void acquire() {
    std::unique_lock<std::mutex> lock(m_);
    cv_.wait(lock, [this]() { return val_; });
    val_ = false;
    lock.unlock();
  }

  void release() {
    std::unique_lock<std::mutex> lock(m_);
    val_ = true;
    lock.unlock();
    cv_.notify_one();
  }

 private:
  std::condition_variable cv_;
  std::mutex m_;
  bool val_ = false;
};

}  // namespace std

#endif  // CTRL_UTILS_SEMAPHORE_H_
