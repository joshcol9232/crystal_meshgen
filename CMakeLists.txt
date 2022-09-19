cmake_minimum_required(VERSION 3.24)
project(crymsh LANGUAGES CXX)

set(PYTHON_INCLUDE_DIR "/usr/include/python3.10/")

add_subdirectory(extern/pybind11)
add_subdirectory(src)
