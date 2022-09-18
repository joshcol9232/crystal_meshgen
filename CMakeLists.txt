cmake_minimum_required(VERSION 3.5)

project(crymsh LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenMP REQUIRED)
find_package (Eigen3 3.4 REQUIRED NO_MODULE)

add_executable(crymsh main.cpp)

target_link_libraries(crymsh PRIVATE OpenMP::OpenMP_CXX Eigen3::Eigen)
