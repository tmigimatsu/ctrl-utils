/**
 * thread_pool.h
 *
 * Copyright 2021. All Rights Reserved.
 *
 * Created: May 25, 2021
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_THREAD_POOL_H_
#define CTRL_UTILS_THREAD_POOL_H_

#include <ctrl_utils/atomic_queue.h>

#include <functional>  // std::function
#include <future>      // std::future, std::promise
#include <thread>      // std::thread
#include <utility>     // std::make_pair, std::move, std::pair
#include <vector>      // std::vector

namespace ctrl_utils {

template <typename T>
class ThreadPool {
 public:
  /**
   * Constructs a thread pool with the specified number of threads.
   *
   * @param num_threads Number of threads to spawn. If zero, spawns the maximum
   *                    number of concurrent threads supported by the hardware.
   */
  ThreadPool(size_t num_threads) {
    if (num_threads == 0) {
      num_threads = std::thread::hardware_concurrency();
    }
    threads_.reserve(num_threads);
    for (size_t i = 0; i < num_threads; i++) {
      threads_.emplace_back([this]() { InfiniteLoop(); });
      // threads_.back().detach();
    }
  }

  /**
   * Terminates the thread pool and joins the threads.
   */
  virtual ~ThreadPool() {
    Terminate();
    for (std::thread& t : threads_) {
      if (t.joinable()) t.join();
    }
  }

  /**
   * Submits a job to the thread pool.
   *
   * @param job Job to submit.
   * @returns Future value returned by the job.
   */
  std::future<T> Submit(std::function<T(void)>&& job) {
    auto promise = std::make_shared<std::promise<T>>();
    jobs_.Push(std::make_pair(std::move(job), promise));
    return promise->get_future();
  }

  std::future<T> Submit(std::function<T(void)>& job) {
    auto promise = std::make_shared<std::promise<T>>();
    jobs_.Push(std::make_pair(job, promise));
    return promise->get_future();
  }

  /**
   * Terminates the thread pool.
   *
   * Jobs that have not yet been initiated will return a future exception.
   */
  void Terminate() {
    terminate_ = true;
    jobs_.Terminate();
  }

 private:
  using Promise = std::shared_ptr<std::promise<T>>;

  void InfiniteLoop() {
    while (!terminate_) {
      std::pair<std::function<T(void)>, Promise> job_promise = jobs_.Pop();
      std::function<T(void)>& job = job_promise.first;
      Promise& promise = job_promise.second;

      // If terminated, raise an exception instead of processing.
      if (terminate_) {
        if (!promise) return;
        promise->set_exception(std::make_exception_ptr(
            std::runtime_error("ThreadPool::Terminate() called.")));
        return;
      }

      // Run the job and return the promised value.
      promise->set_value(job());
    }
  }

  std::vector<std::thread> threads_;

  AtomicQueue<std::pair<std::function<T(void)>, Promise>> jobs_;

  std::sig_atomic_t terminate_ = false;
};

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_THREAD_POOL_H_
