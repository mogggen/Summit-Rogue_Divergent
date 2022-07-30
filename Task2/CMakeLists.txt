cmake_minimum_required(VERSION 3.7)
project(SDL2Test)

find_package(SDL2 REQUIRED)
include_directories(SDL2Test ${SDL2_INCLUDE_DIRS})

add_executable(SDL2Test 
Circle.cc
Circle.h
Point.cc
Point.h
main.cc)
target_link_libraries(SDL2Test ${SDL2_LIBRARIES})
