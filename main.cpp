#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <array>
#include <Eigen/Dense>

using Eigen::Vector3d;
typedef std::array<Vector3d, 3> Triangle;

#define OUT_PRECISION 8

void write_triangle(std::ofstream& fo, const Triangle verts) {
  fo << "facet normal 0e0 0e0 0e0\n\touter loop\n";
  for (char i = 0; i < 3; ++i)
    fo << "\t\tvertex " << std::scientific << verts[i][0] << ' '
                        << verts[i][1] << ' '
                        << verts[i][2]
                        << std::endl;

  fo << "\tendloop\nendfacet\n";
}

Vector3d circle_eq(Vector3d const& v1, Vector3d const& v2, Vector3d const& centre,
                 const double radius, const double a) {
  return centre + radius * v1 * cos(a) + radius * v2 * sin(a);
}

void make_rounded_cylinder(std::vector<Triangle>& triangles, Vector3d const& start,
                           Vector3d const& end, const double radius,
                           unsigned int circle_seg = 32) {

  const Vector3d lengthvec = end - start;
  const Vector3d normal = lengthvec.normalized();

  srand((unsigned int) time(0));
  Vector3d point_on_plane = Vector3d::Random(); // Get random point and find it's place on the plane
  const double d = start.dot(normal);

  if (abs(normal(1)) > 0.00000001) {
    point_on_plane(1) = (d - point_on_plane(2) * normal(2) - point_on_plane(0) * normal(0)) / normal(1);
  } else if (abs(normal(2)) > 0.00000001) {
    point_on_plane(2) = (d - point_on_plane(1) * normal(1) - point_on_plane(0) * normal(0)) / normal(2);
  } else {
    point_on_plane(0) = (d - point_on_plane(1) * normal(1) - point_on_plane(2) * normal(2)) / normal(0);
  }

  // Vectors in plane of cylinder faces (to form circular ends)
  Vector3d v1 = point_on_plane.cross(normal);
  v1.normalize();
  Vector3d v2 = v1.cross(normal);

  const double da = 2 * M_PI/circle_seg;
  double anext = 0;
  Vector3d prev = circle_eq(v1, v2, start, radius, anext);
  for (size_t t = 0; t < circle_seg; ++t) {
    anext += da;
    const Vector3d prevtmp = circle_eq(v1, v2, start, radius, anext); // next segment
    // Circle ends
    triangles.push_back({Vector3d(start), Vector3d(prev), Vector3d(prevtmp)}); // Top
    triangles.push_back({Vector3d(end), prev + lengthvec, prevtmp + lengthvec}); // Bottom
    // Side panels
    triangles.push_back({Vector3d(prev), Vector3d(prevtmp), prevtmp + lengthvec}); // top -> bottom
    triangles.push_back({prevtmp + lengthvec, prev + lengthvec, Vector3d(prev)}); // bottom -> top
    prev = prevtmp;
  }
}


int main() {
  std::vector<Triangle> tris;

  // Write to output stl
  std::ofstream fo;
  fo.open("./output.stl");
  fo.precision(OUT_PRECISION);
  fo << "solid " << std::endl;

  /*
  Vector3d tri[3] = {Vector3d(0, 0, 0), Vector3d(0, 1, 0), Vector3d(0.5, 0.5000001, 0)};
  write_triangle(fo, tri);
  */
  make_rounded_cylinder(tris, Vector3d(0, 0, 0), Vector3d(0, 0, 3), 1.0);
  for (char i = 0; i < tris.size(); ++i)
    write_triangle(fo, tris[i]);

  fo.close();

  return 0;
}
