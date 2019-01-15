/**
 * math.h
 *
 * Copyright 2018. All Rights Reserved.
 *
 * Created: October 7, 2018
 * Authors: Toki Migimatsu
 */

#ifndef CTRLUTILS_MATH_H_
#define CTRLUTILS_MATH_H_

namespace utils {
namespace math {

template <typename T>
T Signum(T x, T epsilon = T(0)) {
  return (x > epsilon) - (x < -epsilon);
}

}  // namespace math
}  // namespace utils

#endif  // CTRLUTILS_MATH_H_
