/**
 * atomic_queue.h
 *
 * Copyright 2018. All Rights Reserved.
 *
 * Created: November 18, 2018
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_ATOMIC_QUEUE_H_
#define CTRL_UTILS_ATOMIC_QUEUE_H_

#include <algorithm>           // std::swap
#include <condition_variable>  // std::condition_variable
#include <csignal>             // std::sig_atomic_t
#include <exception>           // std::runtime_error
#include <mutex>               // std::mutex, std::unique_lock
#include <queue>               // std::queue

namespace ctrl_utils {

template <typename T>
class AtomicQueue {
 public:
  /**
   * Waits until the queue is ready and then pops an item.
   */
  T Pop() {
    T value;
    Pop(value);
    return value;
  }

  /**
   * Waits until the queue is ready and then pops an element.
   */
  void Pop(T& value) {
    std::unique_lock<std::mutex> lock(m_);
    cv_.wait(lock, [this]() { return !queue_.empty() || terminate_; });
    if (terminate_) return;

    T& front = queue_.front();
    std::swap(value, front);
    queue_.pop();
  }

  /**
   * Pushes an item to the queue.
   */
  void Push(const T& item) {
    {
      std::unique_lock<std::mutex> lock(m_);
      queue_.push(item);
    }
    cv_.notify_one();
  }

  /**
   * Pushes an item to the queue.
   */
  void Push(T&& item) {
    {
      std::unique_lock<std::mutex> lock(m_);
      queue_.push(std::move(item));
    }
    cv_.notify_one();
  }

  /**
   * Emplaces an item into the queue.
   */
  template <class... Args>
  void Emplace(Args&&... args) {
    {
      std::unique_lock<std::mutex> lock(m_);
      queue_.emplace(args...);
    }
    cv_.notify_one();
  }

  /**
   * Terminates the queue.
   *
   * All currently waiting threads will receive an empty-initialized T.
   */
  void Terminate() {
    terminate_ = true;
    cv_.notify_all();
  }

 private:
  std::queue<T> queue_;
  std::mutex m_;
  std::condition_variable cv_;
  std::sig_atomic_t terminate_ = false;
};

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_ATOMIC_QUEUE_H_
