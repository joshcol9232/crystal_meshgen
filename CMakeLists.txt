cmake_minimum_required(VERSION 3.5)

project(crymsh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)
find_package (Eigen3 3.4 REQUIRED NO_MODULE)

add_library(Crymsh
  src/crymsh/generator.cc src/crymsh/generator.h
  src/crymsh/triangle.cc src/crymsh/triangle.h
  src/crymsh/interface.h)

target_link_libraries(Crymsh PRIVATE OpenMP::OpenMP_CXX Eigen3::Eigen)

add_subdirectory(src/mains)
