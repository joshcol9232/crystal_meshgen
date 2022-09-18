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

#define OUT_PRECISION 8

struct Triangle {
  Vector3d verts[3];

  Triangle(Vector3d v1, Vector3d v2, Vector3d v3): verts{v1, v2, v3} {}

  void write(std::ofstream& fo) {
    fo << "facet normal 0e0 0e0 0e0\n\touter loop\n";
    for (char i = 0; i < 3; ++i)
      fo << "\t\tvertex " << std::scientific << verts[i][0] << ' '
                          << verts[i][1] << ' '
                          << verts[i][2]
                          << std::endl;

    fo << "\tendloop\nendfacet\n";
  }
};

Vector3d circle_eq(const Vector3d& v1, const Vector3d& v2, const Vector3d& centre,
                 const double radius, const double a) {
  return centre + radius * v1 * cos(a) + radius * v2 * sin(a);
}

void make_hemisphere(std::vector<Triangle>& triangles, const Vector3d& v1,
                     const Vector3d& v2, const Vector3d& centre,
                     const Vector3d& dir, const double r0, const double a0,
                     const double a1, const size_t longitudes) {

  const double dphi = M_PI/(2 * static_cast<double>(longitudes+1));
  double cosphi_next, sinphi_next, r_lower, r_higher;
  double phi = 0;
  double cosphi = cos(phi);
  double sinphi = sin(phi);
  Vector3d vert0, vert1, vert2, vert3;

  for (size_t l = 0; l < longitudes; ++l) {
    cosphi_next = cos(phi + dphi);
    sinphi_next = sin(phi + dphi);
    r_lower = r0 * cosphi;
    r_higher = r0 * cosphi_next;

    vert0 = circle_eq(v1, v2, centre, r_lower, a0) + dir * r0 * sinphi;
    vert1 = circle_eq(v1, v2, centre, r_lower, a1) + dir * r0 * sinphi;
    vert2 = circle_eq(v1, v2, centre, r_higher, a1) + dir * r0 * sinphi_next;
    vert3 = circle_eq(v1, v2, centre, r_higher, a0) + dir * r0 * sinphi_next;

    triangles.push_back(Triangle(Vector3d(vert0), Vector3d(vert1), Vector3d(vert2)));
    triangles.push_back(Triangle(Vector3d(vert0), Vector3d(vert2), Vector3d(vert3)));

    cosphi = cosphi_next;
    sinphi = sinphi_next;
    phi += dphi;
  }
  // Final cap triangle
  const double sinphi_last = sin(M_PI/2.0 - dphi);
  const double cosphi_last = cos(M_PI/2.0 - dphi);
  triangles.push_back(Triangle(
    circle_eq(v1, v2, centre, r0 * cosphi_last, a0) + dir * r0 * sinphi_last,
    circle_eq(v1, v2, centre, r0 * cosphi_last, a1) + dir * r0 * sinphi_last,
    centre + dir * r0  // Top of hemisphere
  ));
}


void make_rounded_cylinder(std::vector<Triangle>& triangles, const Vector3d& start,
                           const Vector3d& end, const double radius,
                           const size_t circle_seg, const size_t long_seg) {

  const Vector3d lengthvec = end - start;
  const Vector3d normal = lengthvec.normalized();

  Vector3d point_on_plane = Vector3d(0.332678436, 0.698741321, 0.2284781651); // Get random point and find it's place on the plane
  const double d = start.dot(normal);

  if (abs(normal(1)) > 0.0000001) {
    point_on_plane(1) = (d - point_on_plane(2) * normal(2) - point_on_plane(0) * normal(0)) / normal(1);
  } else if (abs(normal(2)) > 0.0000001) {
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
    // Rounded ends
    make_hemisphere(triangles, v1, v2, end, normal,
                    radius, anext-da, anext, long_seg); // Bottom
    make_hemisphere(triangles, v1, v2, start, -normal,
                    radius, anext-da, anext, long_seg); // Top

    // Side panels
    triangles.push_back(
      Triangle(Vector3d(prev), Vector3d(prevtmp), prevtmp + lengthvec)
    ); // top -> bottom
    triangles.push_back(
      Triangle(prevtmp + lengthvec, prev + lengthvec, Vector3d(prev))
    ); // bottom -> top
    prev = prevtmp;
  }
}


size_t get_triangle_num(size_t circle_seg, size_t long_seg) {
  return circle_seg * 4 * (1 + long_seg);
}


int main() {
  size_t circle_seg = 128;
  size_t long_seg = 12;
  auto tri_num = get_triangle_num(circle_seg, long_seg);
  std::cout << "Expected triangle count: " << tri_num << std::endl;

  std::vector<Triangle> tris;
  tris.reserve(tri_num);

  // Write to output stl
  std::ofstream fo;
  fo.open("./output.stl");
  fo.precision(OUT_PRECISION);
  fo << "solid " << std::endl;

  make_rounded_cylinder(tris, Vector3d(-1, 0, 0), Vector3d(0, 0, 3), 1.0, circle_seg, long_seg);
  std::cout << "Finished, triangle count: " << tris.size() << std::endl;
  std::cout << static_cast<float>(sizeof(Triangle) * tris.size()) / 1024.0 << " MiB." << std::endl;

  for (size_t i = 0; i < tris.size(); ++i)
    tris[i].write(fo);

  fo.close();

  return 0;
}
