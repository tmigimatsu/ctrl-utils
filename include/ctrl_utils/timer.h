/**
 * timer.h
 *
 * Copyright 2018. All Rights Reserved.
 *
 * Created: July 2, 2018
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_TIMER_H_
#define CTRL_UTILS_TIMER_H_

#include <chrono>  // std::chrono
#include <thread>  // std::this_thread

namespace ctrl_utils {

/**
 * @ingroup cpp_utils
 *
 * Timer utility class for use in control loops.
 *
 * @see Python: spatialdyn.Timer
 */
class Timer {
 public:
  using Seconds = std::chrono::duration<double>;
  using Clock = std::chrono::steady_clock;

  /**
   * Default constructor that sets the loop frequency to 1000 Hz.
   */
  Timer() {}

  /**
   * Constructor that sets the loop frequency.
   *
   * @param frequency Frequency of the timer loop [Hz].
   * @see Python: spatialdyn.Timer.__init__()
   */
  Timer(double frequency) { set_freq(frequency); }

  /**
   * @return Timer loop frequency in Hz.
   * @see Python: spatialdyn.Timer.freq
   */
  double freq() const { return 1 / dt(); }

  /**
   * Set the timer loop frequency in Hz.
   */
  void set_freq(double frequency) { set_dt(1. / frequency); }

  /**
   * @return Timer loop time interval in seconds (1 / frequency).
   * @see Python: spatialdyn.Timer.dt
   */
  double dt() const {
    return std::chrono::duration_cast<Seconds>(dt_interval_).count();
  }

  /**
   * Set the timer loop time interval in seconds (1 / frequency).
   */
  void set_dt(double dt) {
    dt_interval_ = std::chrono::duration_cast<Clock::duration>(Seconds(dt));
  }

  /**
   * @return Current CPU time since epoch in seconds.
   * @see Python: spatialdyn.Timer.time
   */
  double time(bool update = false) const {
    if (update) t_curr_ = Clock::now();
    return std::chrono::duration_cast<Seconds>(t_curr_.time_since_epoch())
        .count();
  }

  /**
   * @return CPU time since last timer reset in seconds.
   * @see Python: spatialdyn.Timer.time_elapsed
   */
  double time_elapsed(bool update = false) const {
    if (update) t_curr_ = Clock::now();
    return std::chrono::duration_cast<Seconds>(t_curr_ - t_start_).count();
  }

  /**
   * @return Simulation time since last timer reset in seconds.
   * @see Python: spatialdyn.Timer.time_sim
   */
  double time_sim() const {
    return std::chrono::duration_cast<Seconds>(t_next_ - t_start_).count();
  }

  /**
   * @return Average frequency since last timer reset in Hz.
   */
  double average_freq() const { return (num_iters() - 1) / time_elapsed(); }

  /**
   * @return Number of loop iterations since last timer reset.
   * @see Python: spatialdyn.Timer.num_iters
   */
  unsigned long long num_iters() const { return num_iters_; }

  /**
   * Reset timer.
   *
   * @see Python: spatialdyn.Timer.reset()
   */
  void Reset() {
    num_iters_ = 0;
    t_start_ = Clock::now();
    t_curr_ = t_start_;
    t_next_ = t_start_ + dt_interval_;
  }

  /**
   * Wait for the next timer loop.
   *
   * @see Python: spatialdyn.Timer.sleep()
   */
  void Sleep() {
    t_curr_ = Clock::now();
    if (!initialized_) {
      t_start_ = t_curr_;
      t_next_ = t_curr_;
      initialized_ = true;
    }

    if (t_curr_ < t_next_) {
      std::this_thread::sleep_for(t_next_ - t_curr_);
    }
    t_next_ += dt_interval_;
    ++num_iters_;
  }

 private:
  /// @cond
  Clock::time_point t_start_;
  mutable Clock::time_point t_curr_;
  Clock::time_point t_next_;
  unsigned long long num_iters_ = 0;

  bool initialized_ = false;
  Clock::duration dt_interval_ = std::chrono::milliseconds(1);
  /// @endcond
};

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_TIMER_H_
