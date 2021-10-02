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

/**
 * General PD control law for reaching a goal position with velocity damping.
 *
 * Outputs a desired acceleration given a desired position, as well as the
 * current position and velocity. This behaves like a damped spring-mass system.
 *
 * Control law:
 *     ddx = -kp (x - x_des) - kv dx
 *
 * @param x Current position as an N-D vector.
 * @param x_des Desired position as an N-D vector.
 * @param dx Current velocity as an N-D vector.
 * @param kp_kv Gains matrix as a 2D vector [kp, kv] or an N x 2 matrix, where
 *        the first and second columns correspond to kp and kv, respectively,
 *        and the N rows represent per-dimension gains.
 * @param ddx_max Optional maximum acceleration due to the position error. This
 *        value clips the acceleration when the distance to the goal is large
 *        and is ignored when less than or equal to 0.
 * @param x_err_out Optional output of the position error.
 *
 * @returns Desired acceleration.
 *
 * @see Python: ctrlutils.pd_control()
 */
template <typename Derived1, typename Derived2, typename Derived3,
          typename Derived4>
inline typename Derived1::PlainObject PdControl(
    const ::Eigen::MatrixBase<Derived1>& x,
    const ::Eigen::MatrixBase<Derived2>& x_des,
    const ::Eigen::MatrixBase<Derived3>& dx,
    const ::Eigen::MatrixBase<Derived4>& kp_kv, double ddx_max = 0.,
    typename Derived1::PlainObject* x_err_out = nullptr) {
  static_assert(
      Derived4::ColsAtCompileTime == 2 || (Derived4::ColsAtCompileTime == 1 &&
                                           Derived4::RowsAtCompileTime == 2),
      "kp_kv must be a vector of size 2 or a matrix of size x.rows() x 2.");

  // Output position error
  typename Derived1::PlainObject x_err;
  x_err = x - x_des;
  if (x_err_out != nullptr) {
    *x_err_out = x_err;
  }

  // Apply kp gains
  if (Derived4::ColsAtCompileTime == 1) {
    x_err = -kp_kv(0) * x_err;
  } else {
    x_err = -kp_kv.block(0, 0, x_err.size(), 1).array() * x_err.array();
  }

  // Limit maximum error
  if (ddx_max > 0. && x_err.norm() > ddx_max) {
    x_err = ddx_max * x_err.normalized();
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

/**
 * Special PD control law for 3D orientations.
 *
 * Outputs a desired acceleration given a desired orientation, as well as the
 * current orientation and angular velocity. This behaves like a damped
 * spring-mass system.
 *
 * Control law:
 *     dw = -kp (quat - quat_des) - kv w
 *
 * @param quat Current orientation as a quaternion.
 * @param quat_des Desired orientation as a quaternion.
 * @param w Current angular velocity as a 4D vector.
 * @param kp_kv Gains matrix as a 2D vector [kp, kv] or a 3 x 2 matrix, where
 *        the first and second columns correspond to kp and kv, respectively,
 *        and the 3 rows represent per-dimension (x, y, z) gains.
 * @param dw_max Optional maximum acceleration due to the orientation error. This
 *        value clips the acceleration when the distance to the goal is large
 *        and is ignored when less than or equal to 0.
 * @param ori_err_out Optional output of the orientation error.
 *
 * @returns Desired angular acceleration.
 *
 * @see Python: ctrlutils.pd_control()
 */
template <typename Derived1, typename Derived2>
inline typename Derived1::PlainObject PdControl(
    const ::Eigen::Quaterniond& quat, const ::Eigen::Quaterniond& quat_des,
    const ::Eigen::MatrixBase<Derived1>& w,
    const ::Eigen::MatrixBase<Derived2>& kp_kv, double dw_max = 0.,
    typename Derived1::PlainObject* ori_err_out = nullptr) {
  static_assert(
      (Derived2::RowsAtCompileTime == 3 && Derived2::ColsAtCompileTime == 2) ||
          (Derived2::RowsAtCompileTime == 2 &&
           Derived2::ColsAtCompileTime == 1),
      "kp_kv must be a vector of size 2 or a matrix of size x.rows() x 2.");

  // Output orientation error
  typename Derived1::PlainObject ori_err;
  ori_err = ctrl_utils::OrientationError(quat, quat_des);
  if (ori_err_out != nullptr) {
    *ori_err_out = ori_err;
  }

  // Apply kp gains
  ori_err = -kp_kv(0) * ori_err;

  // Limit maximum error
  if (dw_max > 0. && ori_err.norm() > dw_max) {
    ori_err = dw_max * ori_err.normalized();
  }

  // Apply kv gains
  return ori_err - kp_kv(1) * w;
}

template <typename Derived1, typename Derived2, typename Derived3, typename Derived4>
inline typename Derived1::PlainObject LookatPdControl(
    const ::Eigen::MatrixBase<Derived1>& vec, const ::Eigen::MatrixBase<Derived2>& vec_des,
    const ::Eigen::MatrixBase<Derived3>& w,
    const ::Eigen::MatrixBase<Derived4>& kp_kv, double dw_max = 0.,
    typename Derived1::PlainObject* ori_err_out = nullptr) {
  static_assert(
      (Derived4::RowsAtCompileTime == 3 && Derived4::ColsAtCompileTime == 2) ||
          (Derived4::RowsAtCompileTime == 2 &&
           Derived4::ColsAtCompileTime == 1),
      "kp_kv must be a vector of size 2 or a matrix of size x.rows() x 2.");

  // Output orientation error
  typename Derived1::PlainObject ori_err;
  ori_err = ctrl_utils::LookatError(vec, vec_des);
  if (ori_err_out != nullptr) {
    *ori_err_out = ori_err;
  }

  // Apply kp gains
  ori_err = -kp_kv(0) * ori_err;

  // Limit maximum error
  if (dw_max > 0. && ori_err.norm() > dw_max) {
    ori_err = dw_max * ori_err.normalized();
  }

  // Apply kv gains
  return ori_err - kp_kv(1) * w;
}

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_CONTROL_H_
