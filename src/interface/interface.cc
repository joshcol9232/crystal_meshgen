#include "pybind11/pybind11.h"
#include <Eigen/Dense>
#include "../crymsh/generator.h"

namespace py = pybind11;

int add(int a, int b) {
  return a + b;
}

void run_gen_cylinder() {
  using Eigen::Vector3d;

  size_t circle_seg = 128;
  size_t long_seg = 12;

  auto generator = crymsh::Generator();
  auto rods = {
    Vector3d(0, 0, 0), Vector3d(0, 0, 1),
    Vector3d(0, 0, 0), Vector3d(0, 1, 0),
  };
  generator.save_crystal_stl("./output.stl", rods, 0.1, circle_seg, long_seg);
}

PYBIND11_MODULE(crymshpy, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function that adds two numbers.");
    m.def("run_gen_cylinder", &run_gen_cylinder, "Gen cylinder.");
}
