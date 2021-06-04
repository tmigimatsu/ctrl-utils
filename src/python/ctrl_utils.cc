/**
 * eigen.cc
 *
 * Copyright 2018. All Rights Reserved.
 *
 * Created: September 8, 2018
 * Authors: Toki Migimatsu
 */

#include <pybind11/eigen.h>
#include <pybind11/iostream.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include "ctrl_utils/control.h"
#include "ctrl_utils/euclidian.h"

namespace ctrl_utils {

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(ctrlutils, m) {
  m.def("orientation_error", &OrientationError, "quat"_a, "quat_des"_a, R"pbdoc(
    Computes the quaternion error between the given orientations.

    This quaternion error is equivalent to the angular velocity required to go
    from `quat_des` to `quat` in one second. Following this angular velocity also
    produces the SLERP interpolation between the two orientations.

    For orientation control, a control law might look like the following:

    ```{.cc}
    Eigen::Matrix3d J_w = AngularJacobian(ab);
    Eigen::Vector3d w_err = opspace::OrientationError(Orientation(ab), quat_des);
    Eigen::Vector3d dw = -kp_ori * w_err - kv_ori * J_w * ab.dq;
    Eigen::VectorXd tau = opspace::InverseDynamics(ab, J_w, dw);
    ```

    Args:
        quat: Current orientation.
        quat_des: Desired orientation.
    Returns:
        Quaternion error.

    .. seealso:: C++: :ctrlutils:`ctrl_utils::OrientationError`.
  )pbdoc");

  m.def("lookat_error", &LookatError, "vec"_a, "vec_des"_a, R"pbdoc(
    Computes the orientation error between two lookat vectors.

    Unlike `ctrlutils.orientation_error()`, this error ignores rotations about
    the lookat vectors, producing the shortest rotation between the two.

    Args:
        quat: Current orientation.
        quat_des: Desired orientation.
    Returns:
        Quaternion error.

    .. seealso:: C++: :ctrlutils:`ctrl_utils::LookatError`.
  )pbdoc");

  m.def("near_quaternion",
        (Eigen::Quaterniond(*)(const Eigen::Quaterniond&,
                               const Eigen::Quaterniond&)) &
            NearQuaternion,
        "quat"_a, "quat_reference"_a, R"pbdoc(
    Computes the quaternion for the given orientation that is closer to the
    reference quaternion.

    Each 3D orientation can be represented by two quaternions, where one
    quaternion has the same 4 elements as the other but negated. Of these two
    quaternions, the one that is "closer" to the reference is the one that has a
    positive dot product with the reference.

    This function may be used in orientation control, where `quat` is the goal
    orientation, and `quat_reference` is the current orientation of the
    controlled body. The effect of controlling the body to the closer quaternion
    is that it will rotate the shortest way to the goal orientation. Using the
    farther quarternion will cause it to rotate the long way around, which may
    be desired to avoid singularities, for example.

    Args:
        quat: Quaternion representing the desired orientation.
        quat_ref: Reference quaternion.
    Returns:
        The quaternion that represents the same orientation as `quat` but has a
        positive dot product with `quat_ref`.

    .. seealso:: C++: :ctrlutils:`ctrl_utils::NearQuaternion`.
  )pbdoc");

  m.def("near_quaternion",
        (Eigen::Quaterniond(*)(Eigen::Ref<const Eigen::Matrix3d>,
                               const Eigen::Quaterniond&)) &
            NearQuaternion,
        "ori"_a, "quat_reference"_a, R"pbdoc(
    Computes the quaternion for the given orientation that is closer to the
    reference quaternion.

    Args:
        ori: 3D matrix representing the desired orientation.
        quat_ref: Reference quaternion.
    Returns:
        The quaternion that represents the same orientation as `ori` but has a
        positive dot product with `quat_ref`.

    .. seealso:: C++: :ctrlutils:`ctrl_utils::NearQuaternion`.
  )pbdoc");

  m.def("far_quaternion",
        (Eigen::Quaterniond(*)(const Eigen::Quaterniond&,
                               const Eigen::Quaterniond&)) &
            FarQuaternion,
        "quat"_a, "quat_reference"_a, R"pbdoc(
    Computes the quaternion for the given orientation that is farther from the
    reference quaternion.

    Args:
        quat: Quaternion representing the desired orientation.
        quat_ref: Reference quaternion.
    Returns:
        The quaternion that represents the same orientation as `quat` but has a
        negative dot product with `quat_ref`.

    .. seealso:: C++: :ctrlutils:`ctrl_utils::FarQuaternion`.
  )pbdoc");

  m.def("far_quaternion",
        (Eigen::Quaterniond(*)(Eigen::Ref<const Eigen::Matrix3d>,
                               const Eigen::Quaterniond&)) &
            FarQuaternion,
        "ori"_a, "quat_reference"_a, R"pbdoc(
    Computes the quaternion for the given orientation that is farther from the
    reference quaternion.

    Args:
        quat: Quaternion representing the desired orientation.
        quat_ref: Reference quaternion.
    Returns:
        The quaternion that represents the same orientation as `quat` but has a
        negative dot product with `quat_ref`.

    .. seealso:: C++: :ctrlutils:`ctrl_utils::FarQuaternion`.
  )pbdoc");

  m.def(
      "pd_control",
      [](Eigen::Ref<const Eigen::VectorXd> x,
         Eigen::Ref<const Eigen::VectorXd> x_des,
         Eigen::Ref<const Eigen::VectorXd> dx,
         Eigen::Ref<const Eigen::MatrixXd> kp_kv,
         double x_err_max) -> std::pair<Eigen::VectorXd, Eigen::VectorXd> {
        if (kp_kv.rows() == 2 && kp_kv.cols() == 1) {
          Eigen::Map<const Eigen::Vector2d> kp_kv_in(kp_kv.data());
          Eigen::VectorXd x_err;
          Eigen::VectorXd ddx = PdControl(x, x_des, dx, kp_kv_in, x_err_max, &x_err);
          return std::make_pair(std::move(ddx), std::move(x_err));
        } else if (kp_kv.rows() == x.rows() && kp_kv.cols() == 2) {
          Eigen::Map<const Eigen::Matrix<double, Eigen::Dynamic, 2>> kp_kv_in(
              kp_kv.data(), kp_kv.rows(), 2);
          Eigen::VectorXd x_err;
          Eigen::VectorXd ddx = PdControl(x, x_des, dx, kp_kv_in, x_err_max, &x_err);
          return std::make_pair(std::move(ddx), std::move(x_err));
        }

        throw std::invalid_argument(
            "pd_control(): kp_kv must be of size [2,] or [n, 2].");
      },
      "x"_a, "x_des"_a, "dx"_a, "kp_kv"_a, "x_err_max"_a = 0., R"pbdoc(
    General PD control law for reaching a goal position with velocity damping.

    Outputs a desired acceleration given a desired position, as well as the
    current position and velocity. This behaves like a damped spring-mass system.

    Control law:
        ddx = -kp (x - x_des) - kv dx

    Args:
        x: Current position as an N-D vector.
        x_des: Desired position as an N-D vector.
        dx: Current velocity as an N-D vector.
        kp_kv: Gains matrix as a 2D vector [kp, kv] or an N x 2 matrix, where
            the first and second columns correspond to kp and kv, respectively,
            and the N rows represent per-dimension gains.
        x_err_max: Optional maximum position error to prevent the acceleration
            from becoming too large when the position error is large. This value
            is ignored when less than or equal to 0.

    Returns:
        2-tuple (ddx, x_err).

    .. seealso C++: :ctrlutils:`ctrl_utils::PdControl`.
        )pbdoc");

  m.def(
      "pd_control",
      [](Eigen::Quaterniond& quat, Eigen::Quaterniond& quat_des,
         Eigen::Ref<const Eigen::Vector3d> w,
         Eigen::Ref<const Eigen::MatrixXd> kp_kv,
         double ori_err_max) -> std::pair<Eigen::Vector3d, Eigen::Vector3d> {
        if (kp_kv.rows() == 2 && kp_kv.cols() == 1) {
          Eigen::Map<const Eigen::Vector2d> kp_kv_in(kp_kv.data());
          Eigen::Vector3d w_err;
          Eigen::Vector3d dw = PdControl(quat, quat_des, w, kp_kv_in, ori_err_max, &w_err);
          return std::make_pair(std::move(dw), std::move(w_err));
        } else if (kp_kv.rows() == 3 && kp_kv.cols() == 2) {
          Eigen::Map<const Eigen::Matrix<double, 3, 2>> kp_kv_in(kp_kv.data());
          Eigen::Vector3d w_err;
          Eigen::Vector3d dw = PdControl(quat, quat_des, w, kp_kv_in, ori_err_max, &w_err);
          return std::make_pair(std::move(dw), std::move(w_err));
        }

        throw std::invalid_argument(
            "pd_control(): kp_kv must be of size [2,] or [3, 2].");
      },
      "quat"_a, "quat_des"_a, "w"_a, "kp_kv"_a, "ori_err_max"_a = 0., R"pbdoc(
    Special PD control law for 3D orientations.

    Outputs a desired acceleration given a desired orientation, as well as the
    current orientation and angular velocity. This behaves like a damped
    spring-mass system.

    Control law:
        dw = -kp (quat - quat_des) - kv w

    Args:
        quat: Current orientation as a quaternion.
        quat_des: Desired orientation as a quaternion.
        w: Current angular velocity as a 4D vector.
        kp_kv: Gains matrix as a 2D vector [kp, kv] or a 3 x 2 matrix, where
            the first and second columns correspond to kp and kv, respectively,
            and the 3 rows represent per-dimension (x, y, z) gains.
        ori_err_max: Optional maximum orientation error to prevent the
           acceleration from becoming too large when the orientation error is
           large. This value is ignored when less than or equal to 0.

    Returns:
        2-tuple (dw, w_err).

    .. seealso C++: :ctrlutils:`ctrl_utils::PdControl`.
      )pbdoc");

  py::add_ostream_redirect(m);
}

}  // namespace ctrl_utils
