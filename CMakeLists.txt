cmake_minimum_required(VERSION 3.0.0)
project(raytracing VERSION 0.1.0 LANGUAGES C)
add_executable(raytracing main.c)
target_link_libraries(raytracing PRIVATE m)