/**
 * yaml.h
 *
 * Copyright 2019. All Rights Reserved.
 *
 * Created: February 27, 2019
 * Authors: Toki Migimatsu
 */

#ifndef CTRL_UTILS_YAML_H_
#define CTRL_UTILS_YAML_H_

#include <yaml-cpp/yaml.h>

#include <Eigen/Eigen>
#include "ctrl_utils/eigen.h"

namespace YAML {

template<typename Scalar, int Rows, int Cols>
struct convert<Eigen::Matrix<Scalar, Rows, Cols>> {

  static Node encode(const Eigen::Matrix<Scalar, Rows, Cols>& rhs) {
    Node node;
    if (rhs.cols() == 1) {
      for (size_t i = 0; i < rhs.size(); i++) {
        node.push_back(rhs(i));
      }
      return node;
    }

    for (size_t i = 0; i < rhs.rows(); i++) {
      Node row;
      for (size_t j = 0; j < rhs.cols(); j++) {
        row.push_back(rhs(i, j));
      }
      node.push_back(row);
    }
    return node;
  }

  static bool decode(const Node& node, Eigen::Matrix<Scalar, Rows, Cols>& rhs) {
    if (!node.IsSequence() || node.size() == 0) return false;

    if (rhs.size() == 0) {
      if (!node[0].IsSequence()) {
        rhs.resize(node.size(), 1);
      } else {
        if (rhs.rows() != 0 && rhs.rows() != node.size() ||
            rhs.cols() != 0 && rhs.cols() != node[0].size()) return false;
        rhs.resize(node.size(), node[0].size());
      }
    }

    if (rhs.cols() == 1) {
      for (size_t i = 0; i < node.size(); i++) {
        rhs(i) = node[i].as<Scalar>();
      }
      return true;
    }

    for (size_t i = 0; i < node.size(); i++) {
      for (size_t j = 0; j < node[i].size(); j++) {
        rhs(i, j) = node[i][j].as<Scalar>();
      }
    }
    return true;
  }

};

template<typename Scalar>
struct convert<Eigen::Quaternion<Scalar>> {

  static Node encode(const Eigen::Quaternion<Scalar>& rhs) {
    Node node;
    node["w"] = rhs.w();
    node["x"] = rhs.x();
    node["y"] = rhs.y();
    node["z"] = rhs.z();
    return node;
  }

  static bool decode(const Node& node, Eigen::Quaternion<Scalar>& rhs) {
    if (!node.IsMap() || !node["w"] || !node["x"] || !node["y"] || !node["z"]) return false;
    rhs.w() = node["w"].as<Scalar>();
    rhs.x() = node["x"].as<Scalar>();
    rhs.y() = node["y"].as<Scalar>();
    rhs.z() = node["z"].as<Scalar>();
    rhs.normalize();
    return true;
  }

};

template<>
struct convert<Eigen::Isometry3d> {

  static Node encode(const Eigen::Isometry3d& rhs) {
    Node node;
    node["pos"] = Eigen::Vector3d(rhs.translation());
    node["ori"] = Eigen::Quaterniond(rhs.linear());
    return node;
  }

  static bool decode(const Node& node, Eigen::Isometry3d& rhs) {
    if (!node.IsMap() || !node["pos"] || !node["ori"]) return false;

    Eigen::Vector3d pos = node["pos"].as<Eigen::Vector3d>();
    Eigen::Quaterniond ori = node["ori"].as<Eigen::Quaterniond>();
    rhs = Eigen::Translation3d(pos) * ori;
    return true;
  }

};

}  // namespace YAML

#endif  // CTRL_UTILS_YAML_H_
