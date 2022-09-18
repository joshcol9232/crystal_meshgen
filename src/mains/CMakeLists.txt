project(crymsh)


add_executable(gen_cylinder gen_cylinder.cc)
target_link_libraries(gen_cylinder PRIVATE Crymsh Eigen3::Eigen)
