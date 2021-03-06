/**
 * eigen.cc
 *
 * Copyright 2018. All Rights Reserved.
 *
 * Created: September 8, 2018
 * Authors: Toki Migimatsu
 */

#include <pybind11/eigen.h>
#include <pybind11/operators.h>
#include <pybind11/pybind11.h>

#include <Eigen/Eigen>

namespace Eigen {

namespace py = pybind11;
using namespace pybind11::literals;

PYBIND11_MODULE(ctrlutils_eigen, m) {
  // Isometry3d
  py::class_<Isometry3d>(m, "Isometry3d")
      .def(py::init<const Isometry3d&>())
      .def(py::init<const Matrix<double, 4, 4>&>())
      .def(py::init<const Matrix3d&>())
      .def(py::init<const Quaterniond&>())
      .def("matrix",
           (Matrix<double, 4, 4> & (Isometry3d::*)(void)) & Isometry3d::matrix,
           py::return_value_policy::reference_internal)
      .def("linear",
           (Block<Matrix<double, 4, 4>, 3, 3>(Isometry3d::*)(void)) &
               Isometry3d::linear,
           py::return_value_policy::reference_internal)
      .def("translation",
           (Block<Matrix<double, 4, 4>, 3, 1, true>(Isometry3d::*)(void)) &
               Isometry3d::translation,
           py::return_value_policy::reference_internal)
      .def("inverse", &Isometry3d::inverse);

  // Translation3d
  py::class_<Translation3d>(m, "Translation3d")
      .def(py::init<const Translation3d&>())
      .def(py::init<const Vector3d&>())
      .def("translation",
           (Vector3d & (Translation3d::*)(void)) & Translation3d::translation,
           py::return_value_policy::reference_internal)
      .def("inverse", &Translation3d::inverse);

  // Quaterniond
  py::class_<Quaterniond>(m, "Quaterniond")
      .def(py::init<const Quaterniond&>())
      .def(py::init<const double&, const double&, const double&,
                    const double&>(), "w"_a, "x"_a, "y"_a, "z"_a)
      .def(py::init<const AngleAxisd&>())
      .def(py::init<const Matrix3d&>())
      .def_property(
          "w", (const double& (Quaterniond::*)(void) const) & Quaterniond::w,
          [](Quaterniond& quat, double w) { quat.w() = w; })
      .def_property(
          "x", (const double& (Quaterniond::*)(void) const) & Quaterniond::x,
          [](Quaterniond& quat, double x) { quat.x() = x; })
      .def_property(
          "y", (const double& (Quaterniond::*)(void) const) & Quaterniond::y,
          [](Quaterniond& quat, double y) { quat.y() = y; })
      .def_property(
          "z", (const double& (Quaterniond::*)(void) const) & Quaterniond::z,
          [](Quaterniond& quat, double z) { quat.z() = z; })
      .def_property(
          "coeffs",
          (const Vector4d& (Quaterniond::*)(void) const) & Quaterniond::coeffs,
          [](Quaterniond& quat, const Eigen::Vector4d& coeffs) {
            quat.coeffs() = coeffs;
          })
      .def("normalized", &Quaterniond::normalized)
      .def("inverse", &Quaterniond::inverse)
      .def("matrix", &Quaterniond::matrix)
      .def("set",
           [](Quaterniond& quat, const Quaterniond& other) { quat = other; })
      .def("__mul__", [](const Quaterniond& quat,
                         const Quaterniond& other) { return quat * other; })
      .def("__repr__", [](const Quaterniond& quat) {
        return "<eigen.Quaterniond (x=" + std::to_string(quat.x()) +
               ", y=" + std::to_string(quat.y()) +
               ", z=" + std::to_string(quat.z()) +
               ", w=" + std::to_string(quat.w()) + ")>";
      });

  // AngleAxisd
  py::class_<AngleAxisd>(m, "AngleAxisd")
      .def(py::init<const AngleAxisd&>())
      .def(py::init<const double&, const Eigen::Vector3d&>(), "angle"_a, "axis"_a)
      .def(py::init<const Quaterniond&>())
      .def(py::init<const Matrix3d&>())
      .def_property("angle",
                    (double (AngleAxisd::*)(void) const) & AngleAxisd::angle,
                    [](AngleAxisd& aa, double angle) { aa.angle() = angle; })
      .def_property(
          "axis",
          (const Eigen::Vector3d& (AngleAxisd::*)(void) const) &
              AngleAxisd::axis,
          [](AngleAxisd& aa, const Eigen::Vector3d& axis) { aa.axis() = axis; })
      .def("inverse", &AngleAxisd::inverse)
      .def("matrix", &AngleAxisd::matrix)
      .def("__mul__", (Quaterniond(AngleAxisd::*)(const Quaterniond&) const) &
                          AngleAxisd::operator*)
      .def("__mul__", (Quaterniond(AngleAxisd::*)(const AngleAxisd&) const) &
                          AngleAxisd::operator*)
      .def("__repr__", [](const Eigen::AngleAxisd& aa) {
        return "<eigen.AngleAxisd (angle=" + std::to_string(aa.angle()) +
               ", axis=[" + std::to_string(aa.axis()[0]) +
               ", " + std::to_string(aa.axis()[1]) +
               ", " + std::to_string(aa.axis()[2]) + "])>";
      });

  // LDLT<Eigen::MatrixXd>
  py::class_<LDLT<MatrixXd>>(m, "LdltMatrixXd")
      .def("solve", [](const LDLT<MatrixXd>& ldlt, const MatrixXd& x) {
        return ldlt.solve(x);
      });
}

}  // namespace Eigen
