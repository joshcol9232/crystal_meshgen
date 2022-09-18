#pragma once

#include <Eigen/Dense>
using Eigen::Vector3d;

namespace crymsh {

struct Triangle {
  Vector3d verts[3];

  Triangle(Vector3d v1, Vector3d v2, Vector3d v3): verts{v1, v2, v3} {}

  void write(std::ofstream& fo);
};

};
