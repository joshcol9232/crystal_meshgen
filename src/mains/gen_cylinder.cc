#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <array>
#include <Eigen/Dense>

#include "../crymsh/generator.h"

using Eigen::Vector3d;


int main() {
  size_t circle_seg = 128;
  size_t long_seg = 12;

  auto generator = crymsh::Generator();
  auto rods = {
    Vector3d(0, 0, 0), Vector3d(0, 0, 1),
    Vector3d(0, 0, 0), Vector3d(0, 1, 0),
  };
  generator.save_crystal_stl("./output.stl", rods, 0.1, circle_seg, long_seg);

  return 0;
}
