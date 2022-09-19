find_package (Eigen3 3.4 REQUIRED NO_MODULE)

include_directories(${PYTHON_INCLUDE_DIR})

pybind11_add_module(crymshpy interface.cc)
target_link_libraries(crymshpy LINK_PUBLIC Crymsh Eigen3::Eigen)
