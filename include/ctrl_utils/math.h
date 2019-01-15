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
namespace math {

template <typename T>
T Signum(T x, T epsilon = T(0)) {
  return (x > epsilon) - (x < -epsilon);
}

}  // namespace math
}  // namespace ctrl_utils

#endif  // CTRL_UTILS_MATH_H_
