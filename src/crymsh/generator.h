#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <Eigen/Dense>

#include "triangle.h"


namespace crymsh {
using Eigen::Vector3d;

class Generator {
public:
  Generator() {}
  void clear();
  void save_crystal_stl(const std::string& filepath,
                        const std::vector<Vector3d>& rods,   // Vectors in groups of 2
                        const double rod_radius, const size_t radial_seg,
                        const size_t longitude_seg, const size_t flush_period);

private:
  void make_hemisphere(const Vector3d& v1, const Vector3d& v2, const Vector3d& centre,
                       const Vector3d& dir, const double r0, const double a0,
                       const double a1, const size_t longitudes);

  void make_rounded_cylinder(const Vector3d& start, const Vector3d& end,
                             const double radius, const size_t circle_seg,
                             const size_t long_seg);

  void flush_to(std::ofstream& fo);

  std::vector<Triangle> triangles;
};

static size_t triangles_per_cylinder(size_t circle_seg, size_t rad_seg);
static Vector3d circle_eq(const Vector3d& v1, const Vector3d& v2, const Vector3d& centre,
                          const double radius, const double a);

}; // namespace crymsh
