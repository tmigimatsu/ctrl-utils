/**
 * euclidian.h
 *
 * Copyright 2019. All Rights Reserved.
 *
 * Created: May 16, 2019
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_EUCLIDIAN_H_
#define CTRL_UTILS_EUCLIDIAN_H_

#include <spatial_dyn/eigen/spatial_math.h>

namespace ctrl_utils {

/**
 * Compute the quaternion error between the given orientations.
 *
 * This quaternion error is equivalent to the angular velocity required to go
 * from `quat_des` to `quat` in one second. Following this angular velocity also
 * produces the SLERP interpolation between the two orientations.
 *
 * For orientation control, a control law might look like the following:
 *
 * ```{.cc}
 * Eigen::Matrix3d J_w = AngularJacobian(ab);
 * Eigen::Vector3d w_err = opspace::OrientationError(Orientation(ab), quat_des);
 * Eigen::Vector3d dw = -kp_ori * w_err - kv_ori * J_w * ab.dq;
 * Eigen::VectorXd tau = opspace::InverseDynamics(ab, J_w, dw);
 * ```
 *
 * @param quat Current orientation.
 * @param quat_des Desired orientation.
 * @return Quaternion error.
 * @see Python: spatialdyn.opspace.orientation_error()
 */
inline Eigen::Vector3d OrientationError(const Eigen::Quaterniond &quat,
                                        const Eigen::Quaterniond &quat_des);

/**
 * Compute the orientation error between two lookat vectors.
 *
 * Unlike opspace::OrientationError(), this error ignores rotations about the
 * lookat vectors, producing the shortest rotation between the two.
 *
 * @param quat Current orientation.
 * @param quat_des Desired orientation.
 * @return Quaternion error.
 * @see Python: spatialdyn.opspace.orientation_error()
 */
inline Eigen::Vector3d LookatError(const Eigen::Vector3d &vec, const Eigen::Vector3d &vec_des);

inline Eigen::Quaterniond NearQuaternion(const Eigen::Quaterniond& quat,
                                         const Eigen::Quaterniond& quat_reference);

inline Eigen::Quaterniond NearQuaternion(Eigen::Ref<const Eigen::Matrix3d> ori,
                                         const Eigen::Quaterniond& quat_reference);

inline Eigen::Quaterniond FarQuaternion(const Eigen::Quaterniond& quat,
                                        const Eigen::Quaterniond& quat_reference);

// Eigen::Matrix<double,4,3> AngularVelocityToQuaternionMap(const Eigen::Quaterniond& quat) {
//   Eigen::Matrix<double,4,3> E;
//   E <<  quat.w(),  quat.z(), -quat.y(),
//        -quat.z(),  quat.w(),  quat.x(),
//         quat.y(), -quat.x(),  quat.w(),
//        -quat.x(), -quat.y(), -quat.z();
//   return 0.5 * E;
// }

// Eigen::Matrix<double,4,3> AngularVelocityToAngleAxisMap(const Eigen::AngleAxisd& aa) {
//   Eigen::Matrix<double,4,3> E;
//   E << aa.axis().transpose(),
//        -0.5 * (std::sin(aa.angle()) / (1 - std::cos(aa.angle())) *
//                ctrl_utils::DoubleCrossMatrix(aa.axis()) +
//                ctrl_utils::CrossMatrix(aa.axis()));
//   return E;
// }

inline Eigen::Vector3d Log(const Eigen::Matrix3d& R);

inline Eigen::Matrix3d Exp(const Eigen::Vector3d& w);

/**
 * d/dx exp(x) = d/de|e=0 log(f(x+e) f(x)^{-1})
 */
inline Eigen::Matrix3d ExpMapDerivative(const Eigen::AngleAxisd& aa);

inline Eigen::Matrix3d LogMapDerivative(const Eigen::AngleAxisd& aa);

template<int Cols>
struct ExpMapJacobianReturnType;

template<>
struct ExpMapJacobianReturnType<1> {
  using type = Eigen::Matrix3d;
};

template<int Cols>
struct ExpMapJacobianReturnType {
  using type = Eigen::Matrix<double,9,Cols>;
};

/**
 * Compute the Jacobian of a rotated matrix R * X with respect to the rotation's
 * exponential coordinates (angle axis representation).
 *
 * If X is a vector, the Jacobian returned is d(Rx)/dw. If X is a matrix, the
 * Jacobian tensor is stacked into a matrix [d(RX)/dw_x; d(RX)/dw_y; d(RX)/dw_z].
 *
 * d/dx exp(x) p = d/dexp(x) exp(x) p * d/dx exp(x)
 * d/dx exp(x) p = d/dk (I + ad_k) Rp * d/de log(f(x+e) f(-x))
 *               = d/dk Rp + [k]x Rp
 *               = d/dk Rp - [Rp]x k
 *               = -[Rp]x
 * d/dx exp(x) p = d/dexp(x) Ad_exp(x) p * d/dx exp(x)
 *               = d/dexp(x) exp(ad_x) p * d/dx exp(x)
 */
template<typename Derived>
inline typename ExpMapJacobianReturnType<Eigen::MatrixBase<Derived>::ColsAtCompileTime>::type
ExpMapJacobian(const Eigen::AngleAxisd& aa,
               const Eigen::MatrixBase<Derived>& X);

inline typename ExpMapJacobianReturnType<3>::type
ExpMapJacobian(const Eigen::AngleAxisd& aa) {
  return ExpMapJacobian(aa, Eigen::Matrix3d::Identity());
}

inline Eigen::Vector6d Log(const Eigen::Isometry3d& T);

inline Eigen::Matrix6d ExpMapDerivative(const Eigen::Isometry3d& T);

inline Eigen::Matrix6d LogMapDerivative(const Eigen::Isometry3d& T);

////////////////////
// Implementation //
////////////////////

Eigen::Vector3d OrientationError(const Eigen::Quaterniond &quat, const Eigen::Quaterniond &quat_des) {
  Eigen::Quaterniond quat_err = quat * quat_des.inverse();
  Eigen::AngleAxisd aa_err(quat_err);  // Angle will always be between [0, pi]
  double angle = (quat_err.w() < 0) ? aa_err.angle() - 2 * M_PI : aa_err.angle();
  return angle * aa_err.axis();
}

Eigen::Vector3d LookatError(const Eigen::Vector3d &vec, const Eigen::Vector3d &vec_des) {
  Eigen::Quaterniond quat_err = Eigen::Quaterniond::FromTwoVectors(vec_des, vec);
  Eigen::AngleAxisd aa_err(quat_err);
  double angle = (quat_err.w() < 0) ? aa_err.angle() - 2 * M_PI : aa_err.angle();
  return angle * aa_err.axis();
}

Eigen::Quaterniond NearQuaternion(const Eigen::Quaterniond& quat,
                                  const Eigen::Quaterniond& quat_reference) {
  Eigen::Quaterniond result = quat;
  if (quat.dot(quat_reference) < 0) result.coeffs() *= -1;
  return result;
}

Eigen::Quaterniond NearQuaternion(Eigen::Ref<const Eigen::Matrix3d> ori,
                                  const Eigen::Quaterniond& quat_reference) {
  Eigen::Quaterniond result(ori);
  if (result.dot(quat_reference) < 0) result.coeffs() *= -1;
  return result;
}

Eigen::Quaterniond FarQuaternion(const Eigen::Quaterniond& quat,
                                 const Eigen::Quaterniond& quat_reference) {
  Eigen::Quaterniond result = quat;
  if (quat.dot(quat_reference) > 0) result.coeffs() *= -1;
  return result;
}

Eigen::Vector3d Log(const Eigen::Matrix3d& R) {
  const Eigen::AngleAxisd aa(R);
  return aa.angle() * aa.axis();
}

Eigen::Matrix3d Exp(const Eigen::Vector3d& w) {
  return Eigen::AngleAxisd(w.norm(), w.normalized()).toRotationMatrix();
}

Eigen::Matrix3d ExpMapDerivative(const Eigen::AngleAxisd& aa) {
  const double theta = aa.angle();
  if (theta == 0.) {
    return Eigen::Matrix3d::Zero();
  } else if (theta * theta < std::numeric_limits<double>::epsilon()) {
    return Eigen::Matrix3d::Identity();
  }
  return Eigen::Matrix3d::Identity() +
      (1. - std::cos(theta)) / theta * CrossMatrix(aa.axis()) +
      (1. - std::sin(theta) / theta) * DoubleCrossMatrix(aa.axis());
}

Eigen::Matrix3d LogMapDerivative(const Eigen::AngleAxisd& aa) {
  const double theta = aa.angle();
  if (theta == 0.) {
    return Eigen::Matrix3d::Zero();
  } else if (theta * theta < std::numeric_limits<double>::epsilon()) {
    return Eigen::Matrix3d::Identity();
  }
  return Eigen::Matrix3d::Identity() - 0.5 * theta * CrossMatrix(aa.axis()) +
      (1. - 0.5 * theta * std::sin(theta) / (1. - std::cos(theta))) * DoubleCrossMatrix(aa.axis());
}

template<typename Derived>
inline typename ExpMapJacobianReturnType<Eigen::MatrixBase<Derived>::ColsAtCompileTime>::type
ExpMapJacobian(const Eigen::AngleAxisd& aa, const Eigen::MatrixBase<Derived>& X) {
  constexpr int Cols = Eigen::MatrixBase<Derived>::ColsAtCompileTime;
  typename ExpMapJacobianReturnType<Cols>::type J;
  const Eigen::Matrix3d dExp = ExpMapDerivative(aa);
  const typename Eigen::MatrixBase<Derived>::PlainObject RX = aa * X;
  for (size_t i = 0; i < 3; i++) {
    Eigen::Map<Eigen::MatrixXd> J_i(J.data() + 3 * RX.cols() * i, 3, RX.cols());
    J_i = RX.colwise().cross(-dExp.col(i));
  }
  return J;
}

Eigen::Vector6d Log(const Eigen::Isometry3d& T) {
  const Eigen::AngleAxisd aa(T.linear());
  const double theta = aa.angle();
  const Eigen::Vector3d w = theta * aa.axis();
  const auto& p = T.translation();
  const Eigen::Vector3d w_x_p = aa.axis().cross(p);
  const double a = 1. - 0.5 * theta * std::sin(theta) / (1. - std::cos(theta));

  Eigen::Vector6d v;
  v.head<3>() = p - 0.5 * theta * w_x_p + a * aa.axis().cross(w_x_p);
  if (theta < 1e-5) v.head<3>().setZero();
  v.tail<3>() = w;
  return v;
}

inline Eigen::Matrix3d ExpMapDerivativeCrossTerm(const Eigen::AngleAxisd& aa,
                                                 Eigen::Ref<const Eigen::Vector3d> x) {
  const double theta = aa.angle();
  const double theta_sq = theta * theta;
  if (theta_sq < std::numeric_limits<double>::epsilon()) {
    return Eigen::Matrix3d::Zero();
  }
  const double s_theta = std::sin(theta);
  const double c_theta = std::cos(theta);
  const double w_dot_u = aa.axis().dot(x);
  const Eigen::Matrix3d x_x = CrossMatrix(x);
  const Eigen::Matrix3d w_x = CrossMatrix(aa.axis());
  const Eigen::Matrix3d w_x_x_x = w_x * x_x;
  return (1. - c_theta) / theta_sq * x_x +
         (theta - s_theta) / theta_sq * (w_x_x_x + w_x_x_x.transpose()) +
         w_dot_u * (s_theta - 2. * (1 - c_theta) / theta) / theta * w_x +
         w_dot_u * (-2. - c_theta + 3 * s_theta / theta) / theta * DoubleCrossMatrix(aa.axis());
}

Eigen::Matrix6d ExpMapDerivative(const Eigen::Isometry3d& T) {
  const Eigen::AngleAxisd aa(T.linear());
  const auto& x = T.translation();
  Eigen::Matrix6d dExp;
  const Eigen::Matrix3d dExp_w = ExpMapDerivative(aa);
  dExp.topLeftCorner<3,3>() = dExp_w;
  dExp.bottomLeftCorner<3,3>().fill(0.);
  dExp.topRightCorner<3,3>() = ExpMapDerivativeCrossTerm(aa, x);
  dExp.bottomRightCorner<3,3>() = dExp_w;
  return dExp;
}

Eigen::Matrix6d LogMapDerivative(const Eigen::Isometry3d& T) {
  const Eigen::AngleAxisd aa(T.linear());
  const auto& x = T.translation();
  Eigen::Matrix6d dExp;
  const Eigen::Matrix3d dLog_w = LogMapDerivative(aa);
  dExp.topLeftCorner<3,3>() = dLog_w;
  dExp.bottomLeftCorner<3,3>().fill(0.);
  dExp.topRightCorner<3,3>() = -dLog_w * ExpMapDerivativeCrossTerm(aa, x) * dLog_w;
  dExp.bottomRightCorner<3,3>() = dLog_w;
  return dExp;
}

}  // namespace ctrl_utils

#endif  // CTRL_UTILS_EUCLIDIAN_H_
