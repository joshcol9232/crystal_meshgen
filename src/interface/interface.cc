#include "pybind11/pybind11.h"
#include "pybind11/numpy.h"
#include <Eigen/Dense>
#include "../crymsh/generator.h"

namespace py = pybind11;

int add(int a, int b) {
  return a + b;
}

void export_stl(const py::str filepath, const py::array_t<double>& rods,
                const py::float_ rod_radius, const py::int_ radial_seg,
                const py::int_ long_seg) {
  using Eigen::Vector3d;

  std::vector<Vector3d> rods_c;
  size_t rod_count = *(rods.shape());

  rods_c.reserve(rod_count * 2);  // 2 Vector3d per rod

  for (size_t i = 0; i < rod_count; ++i) {
    rods_c.push_back(Vector3d(rods.at(i,0,0), rods.at(i,0,1), rods.at(i,0,2)));
    rods_c.push_back(Vector3d(rods.at(i,1,0), rods.at(i,1,1), rods.at(i,1,2)));
  }

  auto generator = crymsh::Generator();
  generator.save_crystal_stl(filepath, rods_c, rod_radius, radial_seg, long_seg, 8);
  rods_c.clear();
}


static void run_gen_cylinder() {
  using Eigen::Vector3d;

  size_t circle_seg = 128;
  size_t long_seg = 12;

  auto generator = crymsh::Generator();
  auto rods = {
    Vector3d(0, 0, 0), Vector3d(0, 0, 1),
    Vector3d(0, 0, 0), Vector3d(0, 1, 0),
  };
  generator.save_crystal_stl("./output.stl", rods, 0.1, circle_seg, long_seg, 8);
}


PYBIND11_MODULE(crymshpy, m) {
    m.doc() = "pybind11 example plugin"; // optional module docstring

    m.def("add", &add, "A function that adds two numbers.");
    m.def("run_gen_cylinder", &run_gen_cylinder, "Gen cylinder.");
    m.def("export_stl", &export_stl, "Export rods as STL.");
}
