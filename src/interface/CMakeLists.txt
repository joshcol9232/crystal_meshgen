find_package (Eigen3 3.4 REQUIRED NO_MODULE)

add_executable(gen_cylinder gen_cylinder.cc)
target_link_libraries(gen_cylinder PRIVATE Crymsh Eigen3::Eigen)

include_directories("/usr/include/python3.10/")

pybind11_add_module(crymshpy interface.cc)
target_link_libraries(crymshpy LINK_PUBLIC Crymsh Eigen3::Eigen)
