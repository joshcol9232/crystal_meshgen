#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <array>
#include <Eigen/Dense>

#include "triangle.h"

using Eigen::Vector3d;

#define OUT_PRECISION 8

class Generator {
public:
  Generator() {}
  void clear();
  void save_crystal_stl(const std::vector<Vector3d>& rods,
                        const double rod_radius, const size_t radial_seg,
                        const size_t longitude_seg);

private:
  std::vector<Triangle> triangles;

};

size_t triangles_per_cylinder(size_t circle_seg, size_t rad_seg);

// }; // namespace generator
