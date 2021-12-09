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

template <int Mode>
void DeclareTransform(pybind11::handle& m, const char* class_name) {
  using Transform3d = Transform<double, 3, Mode>;
  py::class_<Transform3d>(m, class_name)
      .def(py::init<const Transform3d&>())
      .def(py::init<const Matrix<double, 4, 4>&>())
      .def(py::init<const Matrix3d&>())
      .def(py::init<const Quaterniond&>())
      .def(py::init<const AngleAxisd&>())
      .def_static("identity", &Transform3d::Identity)
      .def(
          "matrix",
          (Matrix<double, 4, 4> & (Transform3d::*)(void)) & Transform3d::matrix,
          py::return_value_policy::reference_internal)
      .def("linear",
           (Block<Matrix<double, 4, 4>, 3, 3>(Transform3d::*)(void)) &
               Transform3d::linear,
           py::return_value_policy::reference_internal)
      .def("rotation",
           (Block<Matrix<double, 4, 4>, 3, 3>(Transform3d::*)(void)) &
               Transform3d::rotation,
           py::return_value_policy::reference_internal)
      .def("translation",
           (Block<Matrix<double, 4, 4>, 3, 1, true>(Transform3d::*)(void)) &
               Transform3d::translation,
           py::return_value_policy::reference_internal)
      .def("inverse", &Transform3d::inverse)
      .def("__mul__", [](const Transform3d& T,
                         const Translation3d& other) { return T * other; })
      .def("__mul__", [](const Transform3d& T,
                         const Vector3d& other) { return T * other; })
      .def("__mul__",
           [](const Transform3d& T, const Matrix<double, 4, 4>& other) {
             return T * other;
           })
      .def("__mul__", [](const Transform3d& T,
                         const Matrix3d& other) { return T * other; })
      .def("__mul__", [](const Transform3d& T,
                         const Quaterniond& other) { return T * other; })
      .def("__mul__", [](const Transform3d& T,
                         const AngleAxisd& other) { return T * other; })
      .def("__repr__", [class_name](const Transform3d& T) {
        return "<eigen." + std::string(class_name) + "\n [[" +
               std::to_string(T.matrix()(0, 0)) + ", " +
               std::to_string(T.matrix()(0, 1)) + ", " +
               std::to_string(T.matrix()(0, 2)) + ", " +
               std::to_string(T.matrix()(0, 3)) + "]\n  [" +
               std::to_string(T.matrix()(1, 0)) + ", " +
               std::to_string(T.matrix()(1, 1)) + ", " +
               std::to_string(T.matrix()(1, 2)) + ", " +
               std::to_string(T.matrix()(1, 3)) + "]\n  [" +
               std::to_string(T.matrix()(2, 0)) + ", " +
               std::to_string(T.matrix()(2, 1)) + ", " +
               std::to_string(T.matrix()(2, 2)) + ", " +
               std::to_string(T.matrix()(2, 3)) + "]\n  [" +
               std::to_string(T.matrix()(3, 0)) + ", " +
               std::to_string(T.matrix()(3, 1)) + ", " +
               std::to_string(T.matrix()(3, 2)) + ", " +
               std::to_string(T.matrix()(3, 3)) + "]]>";
      });
}

PYBIND11_MODULE(ctrlutils_eigen, m) {
  DeclareTransform<Eigen::Isometry>(m, "Isometry3d");
  DeclareTransform<Eigen::Affine>(m, "Affine3d");

  // Translation3d
  py::class_<Translation3d>(m, "Translation3d")
      .def(py::init<>())
      .def(py::init<const Translation3d&>())
      .def(py::init<const Vector3d&>())
      .def_static("identity", &Translation3d::Identity)
      .def("translation",
           (Vector3d & (Translation3d::*)(void)) & Translation3d::translation,
           py::return_value_policy::reference_internal)
      .def("inverse", &Translation3d::inverse)
      .def("__mul__", [](const Translation3d& T,
                         const Translation3d& other) { return T * other; })
      .def("__mul__", [](const Translation3d& T,
                         const Vector3d& other) { return T * other; })
      .def("__mul__", [](const Translation3d& T,
                         const Matrix3d& other) { return T * other; })
      .def("__mul__", [](const Translation3d& T,
                         const Quaterniond& other) { return T * other; })
      .def("__mul__", [](const Translation3d& T,
                         const AngleAxisd& other) { return T * other; })
      .def("__repr__", [](const Translation3d& T) {
        return "<eigen.Translation3d [" + std::to_string(T.translation()[0]) +
               ", " + std::to_string(T.translation()[1]) + ", " +
               std::to_string(T.translation()[2]) + "]>";
      });

  // Quaterniond
  py::class_<Quaterniond>(m, "Quaterniond")
      .def(py::init<>())
      .def(py::init<const AngleAxisd&>())
      .def(py::init<const Matrix3d&>())
      .def(py::init<const Vector4d&>())
      .def(py::init<const Quaterniond&>())
      .def(py::init<const double&, const double&, const double&,
                    const double&>(),
           "w"_a, "x"_a, "y"_a, "z"_a)
      .def_static("from_two_vectors",
                  [](const Eigen::Vector3d& a, const Eigen::Vector3d& b) {
                    return Quaterniond::FromTwoVectors(a, b);
                  })
      .def_static("unit_random", &Quaterniond::UnitRandom)
      .def_static("identity", &Quaterniond::Identity)
      .def("angular_distance",
           [](const Quaterniond& quat, const Quaterniond& other) {
             return quat.angularDistance(other);
           })
      .def_property(
          "coeffs",
          (const Vector4d& (Quaterniond::*)(void) const) & Quaterniond::coeffs,
          [](Quaterniond& quat, const Eigen::Vector4d& coeffs) {
            quat.coeffs() = coeffs;
          })
      .def("conjugate", &Quaterniond::conjugate)
      .def("dot", [](const Quaterniond& quat,
                     const Quaterniond& other) { return quat.dot(other); })
      .def("inverse", &Quaterniond::inverse)
      .def(
          "is_approx",
          [](const Quaterniond& quat, const Quaterniond& other, double prec) {
            return quat.isApprox(other, prec);
          },
          py::arg(), "prec"_a = NumTraits<double>::dummy_precision())
      .def("norm", &Quaterniond::norm)
      .def("normalize", &Quaterniond::normalize)
      .def("normalized", &Quaterniond::normalized)
      .def("set", [](Quaterniond& quat,
                     const Quaterniond& other) { return quat = other; })
      .def("set", [](Quaterniond& quat,
                     const AngleAxisd& other) { return quat = other; })
      .def("set", [](Quaterniond& quat,
                     const Matrix3d& other) { return quat = other; })
      .def("set", [](Quaterniond& quat,
                     const Vector4d& other) { return quat = other; })
      .def("set_from_two_vectors",
           [](Quaterniond& quat, const Vector3d& a, const Vector3d& b) {
             return quat.setFromTwoVectors(a, b);
           })
      .def("set_identity", &Quaterniond::setIdentity)
      .def("slerp",
           [](const Quaterniond& quat, double t, const Quaterniond& other) {
             return quat.slerp(t, other);
           })
      .def("squared_norm", &Quaterniond::squaredNorm)
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
      .def("inverse", &Quaterniond::inverse)
      .def("matrix", &Quaterniond::matrix)
      .def("__mul__", [](const Quaterniond& quat,
                         const Quaterniond& other) { return quat * other; })
      .def("__mul__", [](const Quaterniond& quat,
                         const Vector3d& other) { return quat * other; })
      .def("__mul__", [](const Quaterniond& quat,
                         const Matrix3d& other) { return quat * other; })
      .def("__mul__", [](const Quaterniond& quat,
                         const Translation3d& other) { return quat * other; })
      .def("__mul__", [](const Quaterniond& quat,
                         const Isometry3d& other) { return quat * other; })
      .def("__repr__", [](const Quaterniond& quat) {
        return "<eigen.Quaterniond (x=" + std::to_string(quat.x()) +
               ", y=" + std::to_string(quat.y()) +
               ", z=" + std::to_string(quat.z()) +
               ", w=" + std::to_string(quat.w()) + ")>";
      });

  // AngleAxisd
  py::class_<AngleAxisd>(m, "AngleAxisd")
      .def(py::init<>())
      .def(py::init<const AngleAxisd&>())
      .def(py::init<const Matrix3d&>())
      .def(py::init<const Quaterniond&>())
      .def(py::init<const double&, const Eigen::Vector3d&>(), "angle"_a,
           "axis"_a)
      .def_static("identity", &AngleAxisd::Identity)
      .def_property("angle",
                    (double(AngleAxisd::*)(void) const) & AngleAxisd::angle,
                    [](AngleAxisd& aa, double angle) { aa.angle() = angle; })
      .def_property(
          "axis",
          (const Eigen::Vector3d& (AngleAxisd::*)(void) const) &
              AngleAxisd::axis,
          [](AngleAxisd& aa, const Eigen::Vector3d& axis) { aa.axis() = axis; })
      .def("inverse", &AngleAxisd::inverse)
      .def(
          "is_approx",
          [](const AngleAxisd& aa, const AngleAxisd& other, double prec) {
            return aa.isApprox(other, prec);
          },
          py::arg(), "prec"_a = NumTraits<double>::dummy_precision())
      .def("matrix", &AngleAxisd::matrix)
      .def("__mul__", (Quaterniond(AngleAxisd::*)(const AngleAxisd&) const) &
                          AngleAxisd::operator*)
      .def("__mul__", (Quaterniond(AngleAxisd::*)(const Quaterniond&) const) &
                          AngleAxisd::operator*)
      .def("__mul__", [](const AngleAxisd& aa,
                         const Matrix3d& other) { return aa * other; })
      .def("__mul__", [](const AngleAxisd& aa,
                         const Translation3d& other) { return aa * other; })
      .def("__mul__", [](const AngleAxisd& aa,
                         const Isometry3d& other) { return aa * other; })
      .def("set",
           [](AngleAxisd& aa, const AngleAxisd& other) { return aa = other; })
      .def("set",
           [](AngleAxisd& aa, const AngleAxisd& other) { return aa = other; })
      .def("set",
           [](AngleAxisd& aa, const Matrix3d& other) { return aa = other; })
      .def("__repr__", [](const Eigen::AngleAxisd& aa) {
        return "<eigen.AngleAxisd (angle=" + std::to_string(aa.angle()) +
               ", axis=[" + std::to_string(aa.axis()[0]) + ", " +
               std::to_string(aa.axis()[1]) + ", " +
               std::to_string(aa.axis()[2]) + "])>";
      });

  // LDLT<Eigen::MatrixXd>
  py::class_<LDLT<MatrixXd>>(m, "LdltMatrixXd")
      .def("solve", [](const LDLT<MatrixXd>& ldlt, const MatrixXd& x) {
        return ldlt.solve(x);
      });
}

}  // namespace Eigen
