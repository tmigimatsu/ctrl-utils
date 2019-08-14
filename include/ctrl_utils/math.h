/**
 * math.h
 *
 * Copyright 2018. All Rights Reserved.
 *
 * Created: October 7, 2018
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_MATH_H_
#define CTRL_UTILS_MATH_H_

namespace ctrl_utils {

template<typename T>
T Signum(T x, T epsilon = T(0)) {
  return (x > epsilon) - (x < -epsilon);
}

template<typename T>
T Clip(T x, T max) {
  return (x > max) ? max : (x < -max) ? -max : x;
}

template<typename T>
T Power(T x, size_t exp) {
  if (exp == 0) return 1;
  T result = x;
  for (size_t i = 1; i < exp; i++) {
    result *= x;
  }
  return result;
}

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_MATH_H_
