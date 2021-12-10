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
#include <memory>  // std::unique_ptr
#include <string>  // std::string, std:;to_string

namespace py = pybind11;
using namespace ::pybind11::literals;
using namespace ::Eigen;

template <int Mode>
void DeclareTransform(pybind11::module& m, const char* class_name) {
  using Transform3d = Transform<double, 3, Mode>;
  py::class_<Transform3d> c(m, class_name);
  c.def(py::init<const Transform3d&>())
      .def(py::init<Ref<const Matrix4d>>())
      .def(py::init<Ref<const Matrix3d>>())
      .def(py::init<const Quaterniond&>())
      .def(py::init<const AngleAxisd&>())
      .def_property(
          "matrix",
          (const Matrix4d& (Transform3d::*)(void) const) & Transform3d::matrix,
          [](Transform3d& T, Ref<const Matrix4d> matrix) {
            T.matrix() = matrix;
          })
      .def_property(
          "linear",
          (const Block<const Matrix4d, 3, 3> (Transform3d::*)(void) const) &
              Transform3d::linear,
          [](Transform3d& T, Ref<const Matrix3d> linear) {
            T.linear() = linear;
          })
      .def_property(
          "affine",
          (const Block<const Matrix4d, 3, 4> (Transform3d::*)(void) const) &
              Transform3d::affine,
          [](Transform3d& T, Ref<const Matrix<double, 3, 4>> affine) {
            T.affine() = affine;
          })
      .def_property(
          "translation",
          (const Block<const Matrix4d, 3, 1, true> (Transform3d::*)(void)
               const) &
              Transform3d::translation,
          [](Transform3d& T, Ref<const Matrix<double, 3, 1>> translation) {
            T.translation() = translation;
          })
      .def("__mul__", [](const Transform3d& T,
                         const Translation3d& other) { return T * other; })
      .def("__mul__", [](const Transform3d& T,
                         const Quaterniond& other) { return T * other; })
      .def("__mul__", [](const Transform3d& T,
                         const AngleAxisd& other) { return T * other; })
      .def("__mul__", [](const Transform3d& T,
                         const Isometry3d& other) { return T * other; })
      .def("__mul__", [](const Transform3d& T,
                         const Affine3d& other) { return T * other; })
      .def("__mul__", [](const Transform3d& T,
                         const Projective3d& other) { return T * other; })
      .def("__mul__", [](const Transform3d& T,
                         Ref<const Vector4d> other) { return T * other; })
      .def("__mul__", [](const Transform3d& T,
                         Ref<const Matrix4d> other) { return T * other; })
      .def("set",
           [](Transform3d& T, const Translation3d& other) { return T = other; })
      .def("set",
           [](Transform3d& T, const Quaterniond& other) { return T = other; })
      .def("set",
           [](Transform3d& T, const AngleAxisd& other) { return T = other; })
      .def("set",
           [](Transform3d& T, const Transform3d& other) { return T = other; })
      .def("set",
           [](Transform3d& T, const Matrix4d& other) { return T = other; })
      .def("set_identity", &Transform3d::setIdentity)
      .def_static("identity", &Transform3d::Identity)
      // .def("scale",
      //      (Transform3d & (Transform3d::*)(const double&)) &
      //      Transform3d::scale)
      // .def("prescale", (Transform3d & (Transform3d::*)(const double&)) &
      //                      Transform3d::prescale)
      // .def("scale", [](Transform3d& T,
      //                  Ref<const Vector3d> scale) { return T.scale(scale); })
      // .def("prescale",
      //      [](Transform3d& T, Ref<const Vector3d> prescale) {
      //        return T.prescale(prescale);
      //      })
      .def("rotate", &Transform3d::template rotate<Quaterniond>)
      .def("rotate", &Transform3d::template rotate<AngleAxisd>)
      .def("prerotate", &Transform3d::template prerotate<Quaterniond>)
      .def("prerotate", &Transform3d::template prerotate<AngleAxisd>)
      // .def("shear", &Transform3d::shear)
      // .def("preshear", &Transform3d::preshear)
      .def("rotation", &Transform3d::rotation,
           Mode == Isometry ? py::return_value_policy::reference_internal
                            : py::return_value_policy::automatic)
      .def("inverse", &Transform3d::inverse)
      .def("is_approx", &Transform3d::isApprox, py::arg(),
           "prec"_a = NumTraits<double>::dummy_precision())
      .def("make_affine", &Transform3d::makeAffine)
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

  if constexpr (Mode != Projective) {
    c.def("__mul__", [](const Transform3d& T, Ref<const Vector3d> other) {
       return T * other;
     }).def("__mul__", [](const Transform3d& T, Ref<const Matrix3d> other) {
      return T * other;
    });
  }
}

PYBIND11_MODULE(ctrlutils_eigen, m) {
  DeclareTransform<Isometry>(m, "Isometry3d");
  DeclareTransform<Affine>(m, "Affine3d");
  DeclareTransform<Projective>(m, "Projective3d");

  // Translation3d
  py::class_<Translation3d>(m, "Translation3d")
      .def(py::init<>())
      .def(py::init<const Translation3d&>())
      .def(py::init<const double&, const double&, const double&>())
      .def(py::init<Ref<const Vector3d>>())
      .def_property("x",
                    (double(Translation3d::*)(void) const) & Translation3d::x,
                    [](Translation3d& T, double x) { T.x() = x; })
      .def_property("y",
                    (double(Translation3d::*)(void) const) & Translation3d::y,
                    [](Translation3d& T, double y) { T.y() = y; })
      .def_property("z",
                    (double(Translation3d::*)(void) const) & Translation3d::z,
                    [](Translation3d& T, double z) { T.z() = z; })
      .def_property("vector",
                    (const Vector3d& (Translation3d::*)(void) const) &
                        Translation3d::vector,
                    [](Translation3d& T, Ref<const Vector3d> vector) {
                      T.vector() = vector;
                    })
      .def_property("translation",
                    (const Vector3d& (Translation3d::*)(void) const) &
                        Translation3d::translation,
                    [](Translation3d& T, Ref<const Vector3d> translation) {
                      T.translation() = translation;
                    })
      .def("__mul__", [](const Translation3d& T,
                         const Translation3d& other) { return T * other; })
      .def("__mul__", [](const Translation3d& T,
                         const Quaterniond& other) { return T * other; })
      .def("__mul__", [](const Translation3d& T,
                         const AngleAxisd& other) { return T * other; })
      .def("__mul__", [](const Translation3d& T,
                         const Isometry3d& other) { return T * other; })
      .def("__mul__", [](const Translation3d& T,
                         const Affine3d& other) { return T * other; })
      .def("__mul__", [](const Translation3d& T,
                         const Projective3d& other) { return T * other; })
      .def("__mul__", [](const Translation3d& T,
                         const Vector3d& other) { return T * other; })
      .def("__mul__", [](const Translation3d& T,
                         Ref<const Matrix3d> other) { return T * other; })
      .def("set", [](Translation3d& T,
                     const Translation3d& other) { return T = other; })
      .def("inverse", &Translation3d::inverse)
      .def_static("identity", &Translation3d::Identity)
      .def("is_approx", &Translation3d::isApprox, py::arg(),
           "prec"_a = NumTraits<double>::dummy_precision())
      .def("__repr__", [](const Translation3d& T) {
        return "<eigen.Translation3d [" + std::to_string(T.translation()[0]) +
               ", " + std::to_string(T.translation()[1]) + ", " +
               std::to_string(T.translation()[2]) + "]>";
      });

  // Quaterniond
  py::class_<QuaternionBase<Quaterniond>,
             std::unique_ptr<QuaternionBase<Quaterniond>, py::nodelete>>(
      m, "_QuaternionBase_Quaterniond");

  py::class_<Quaterniond, QuaternionBase<Quaterniond>>(m, "Quaterniond")
      .def(py::init<>())
      .def(py::init<const AngleAxisd&>())
      .def(py::init<Ref<const Matrix3d>>())
      .def(py::init<Ref<const Vector4d>>())
      .def(py::init<const Quaterniond&>())
      .def(py::init<const double&, const double&, const double&,
                    const double&>(),
           "w"_a, "x"_a, "y"_a, "z"_a)
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
          "w", (const double& (Quaterniond::*)(void) const) & Quaterniond::w,
          [](Quaterniond& quat, double w) { quat.w() = w; })
      .def_property(
          "vec",
          (const VectorBlock<const Vector4d, 3> (Quaterniond::*)(void) const) &
              Quaterniond::vec,
          [](Quaterniond& quat, Ref<Vector3d> vec) { quat.vec() = vec; })
      .def_property(
          "coeffs",
          (const Vector4d& (Quaterniond::*)(void) const) & Quaterniond::coeffs,
          [](Quaterniond& quat, Ref<const Vector4d> coeffs) {
            quat.coeffs() = coeffs;
          })
      .def("set", [](Quaterniond& quat,
                     const Quaterniond& other) { return quat = other; })
      .def("set", [](Quaterniond& quat,
                     const AngleAxisd& other) { return quat = other; })
      .def("set", [](Quaterniond& quat,
                     Ref<const Matrix3d> other) { return quat = other; })
      .def("set",
           [](Quaterniond& quat, Ref<Vector4d> other) { return quat = other; })
      .def_static("unit_random", &Quaterniond::UnitRandom)
      .def_static("from_two_vectors",
                  [](Ref<const Vector3d> a, Ref<const Vector3d> b) {
                    return Quaterniond::FromTwoVectors(a, b);
                  })
      .def("set_from_two_vectors",
           [](Quaterniond& quat, Ref<const Vector3d> a, Ref<const Vector3d> b) {
             return quat.setFromTwoVectors(a, b);
           })
      .def_static("identity", &Quaterniond::Identity)
      .def("set_identity", &Quaterniond::setIdentity)
      .def("squared_norm", &Quaterniond::squaredNorm)
      .def("norm", &Quaterniond::norm)
      .def("normalize", &Quaterniond::normalize)
      .def("normalized", &Quaterniond::normalized)
      .def("dot", &Quaterniond::dot<Quaterniond>)
      .def("angular_distance", &Quaterniond::angularDistance<Quaterniond>)
      .def("matrix", &Quaterniond::matrix)
      .def("__mul__", &Quaterniond::operator*<Quaterniond>)
      .def("__mul__", [](const Quaterniond& quat,
                         Ref<const Vector3d> other) { return quat * other; })
      .def("__mul__", [](const Quaterniond& quat,
                         Ref<const Matrix3d> other) { return quat * other; })
      .def("__mul__", [](const Quaterniond& quat,
                         const Translation3d& other) { return quat * other; })
      .def("__mul__", [](const Quaterniond& quat,
                         const Isometry3d& other) { return quat * other; })
      .def("__mul__", [](const Quaterniond& quat,
                         const Affine3d& other) { return quat * other; })
      .def("inverse", &Quaterniond::inverse)
      .def("conjugate", &Quaterniond::conjugate)
      .def("slerp", &Quaterniond::slerp<Quaterniond>)
      .def("is_approx", &Quaterniond::isApprox<Quaterniond>, py::arg(),
           "prec"_a = NumTraits<double>::dummy_precision())
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
      .def(py::init<Ref<const Matrix3d>>())
      .def(py::init<const Quaterniond&>())
      .def(py::init<double, Ref<const Vector3d>>(), "angle"_a, "axis"_a)
      .def_property("angle",
                    (double(AngleAxisd::*)(void) const) & AngleAxisd::angle,
                    [](AngleAxisd& aa, double angle) { aa.angle() = angle; })
      .def_property(
          "axis",
          (const Vector3d& (AngleAxisd::*)(void) const) & AngleAxisd::axis,
          [](AngleAxisd& aa, Ref<const Vector3d> axis) { aa.axis() = axis; })
      .def("__mul__", (Quaterniond(AngleAxisd::*)(const AngleAxisd&) const) &
                          AngleAxisd::operator*)
      .def("__mul__", (Quaterniond(AngleAxisd::*)(const Quaterniond&) const) &
                          AngleAxisd::operator*)
      .def("__mul__", [](const AngleAxisd& aa,
                         Ref<const Vector3d> other) { return aa * other; })
      .def("__mul__", [](const AngleAxisd& aa,
                         Ref<const Matrix3d> other) { return aa * other; })
      .def("__mul__", [](const AngleAxisd& aa,
                         const Translation3d& other) { return aa * other; })
      .def("__mul__", [](const AngleAxisd& aa,
                         const Isometry3d& other) { return aa * other; })
      .def("__mul__", [](const AngleAxisd& aa,
                         const Affine3d& other) { return aa * other; })
      .def("set",
           [](AngleAxisd& aa, const AngleAxisd& other) { return aa = other; })
      .def("set",
           [](AngleAxisd& aa, const Quaterniond& other) { return aa = other; })
      .def("set",
           [](AngleAxisd& aa, Ref<const Matrix3d> other) { return aa = other; })
      .def("matrix", &AngleAxisd::matrix)
      .def_static("identity", &AngleAxisd::Identity)
      .def("inverse", &AngleAxisd::inverse)
      .def("is_approx", &AngleAxisd::isApprox, py::arg(),
           "prec"_a = NumTraits<double>::dummy_precision())
      .def("__repr__", [](const Eigen::AngleAxisd& aa) {
        return "<eigen.AngleAxisd (angle=" + std::to_string(aa.angle()) +
               ", axis=[" + std::to_string(aa.axis()[0]) + ", " +
               std::to_string(aa.axis()[1]) + ", " +
               std::to_string(aa.axis()[2]) + "])>";
      });

  // LDLT<Eigen::MatrixXd>
  py::class_<LDLT<MatrixXd>>(m, "LdltMatrixXd")
      .def("solve", [](const LDLT<MatrixXd>& ldlt, Ref<const MatrixXd> x) {
        return ldlt.solve(x);
      });
}
