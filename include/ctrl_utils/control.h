/**
 * control.h
 *
 * Copyright 2019. All Rights Reserved.
 *
 * Created: February 07, 2019
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_CONTROL_H_
#define CTRL_UTILS_CONTROL_H_

#include "ctrl_utils/eigen.h"
#include "ctrl_utils/euclidian.h"

namespace ctrl_utils {

// General PD control law
template<typename Derived1, typename Derived2, typename Derived3, typename Derived4>
inline typename Derived1::PlainObject PdControl(const ::Eigen::MatrixBase<Derived1>& x,
                                                const ::Eigen::MatrixBase<Derived2>& x_des,
                                                const ::Eigen::MatrixBase<Derived3>& dx,
                                                const ::Eigen::MatrixBase<Derived4>& kp_kv,
                                                double x_err_max = 0.,
                                                typename Derived1::PlainObject* p_x_err = nullptr) {
  static_assert(Derived4::ColsAtCompileTime == 2 ||
                (Derived4::ColsAtCompileTime == 1 && Derived4::RowsAtCompileTime == 2),
                "kp_kv must be a vector of size 2 or a matrix of size x.rows() x 2.");

  // Output position error
  typename Derived1::PlainObject x_err;
  x_err = x - x_des;
  if (p_x_err != nullptr) {
    *p_x_err = x_err;
  }

  // Apply kp gains
  if (Derived4::ColsAtCompileTime == 1) {
    x_err = -kp_kv(0) * x_err;
  } else {
    x_err = -kp_kv.block(0, 0, x_err.size(), 1).array() * x_err.array();
  }

  // Limit maximum error
  if (x_err_max > 0. && x_err.norm() > x_err_max) {
    x_err = x_err_max * x_err.normalized();
  }

  // Apply kv gains
  if (Derived4::ColsAtCompileTime == 1) {
    return x_err - kp_kv(1) * dx;
  } else {
    return x_err.array() - kp_kv.block(0, 1, dx.size(), 1).array() * dx.array();
  }

  // With velocity clipping
  // if (dx_max > 0.) {
  //   typename Derived1::PlainObject dx_des = -(kp_kv(0) / kp_kv(1)) * x_err;
  //   if (dx_des.norm() > 0.01) {
  //     typename Derived1::Scalar v = dx_max / dx_des.norm();
  //     if (v > 1.) v = 1.;
  //     return -kp_kv(1) * (dx - v * dx_des);
  //   }
  // }
}

// Special PD control law for orientation
template<typename Derived1>
inline typename Derived1::PlainObject PdControl(const ::Eigen::Quaterniond& quat,
                                                const ::Eigen::Quaterniond& quat_des,
                                                const ::Eigen::MatrixBase<Derived1>& w,
                                                ::Eigen::Ref<const ::Eigen::Vector2d> kp_kv,
                                                double ori_err_max = 0.,
                                                typename Derived1::PlainObject* p_ori_err = nullptr) {
  // Output orientation error
  typename Derived1::PlainObject ori_err;
  ori_err = ctrl_utils::OrientationError(quat, quat_des);
  if (p_ori_err != nullptr) {
    *p_ori_err = ori_err;
  }

  // Apply kp gains
  ori_err = -kp_kv(0) * ori_err;

  // Limit maximum error
  if (ori_err_max > 0. && ori_err.norm() > ori_err_max) {
    ori_err = ori_err_max * ori_err.normalized();
  }

  // Apply kv gains
  return ori_err - kp_kv(1) * w;
}

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_CONTROL_H_
