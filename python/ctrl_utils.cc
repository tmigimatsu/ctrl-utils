/**
 * eigen.cc
 *
 * Copyright 2018. All Rights Reserved.
 *
 * Created: September 8, 2018
 * Authors: Toki Migimatsu
 */

#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include <pybind11/operators.h>

#include "ctrl_utils/euclidian.h"

namespace ctrl_utils {

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(ctrlutils, m) {

  m.def("orientation_error", &OrientationError, "quat"_a, "quat_des"_a);

  m.def("lookat_error", &LookatError, "vec"_a, "vec_des"_a);

  m.def("near_quaternion", (Eigen::Quaterniond (*)(const Eigen::Quaterniond&, const Eigen::Quaterniond&)) &NearQuaternion, "quat"_a, "quat_reference"_a);

  m.def("near_quaternion", (Eigen::Quaterniond (*)(Eigen::Ref<const Eigen::Matrix3d>, const Eigen::Quaterniond&)) &NearQuaternion, "ori"_a, "quat_reference"_a);

  m.def("far_quaternion", &FarQuaternion, "quat"_a, "quat_reference"_a);

}

}  // namespace ctrl_utils
