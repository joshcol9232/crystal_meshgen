#find_package(OpenMP REQUIRED)
find_package (Eigen3 3.4 REQUIRED NO_MODULE)

set(CMAKE_CXX_FLAGS "-fPIC")

add_library(Crymsh
  generator.cc generator.h
  triangle.cc triangle.h)

target_include_directories(Crymsh PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(Crymsh PRIVATE Eigen3::Eigen)
