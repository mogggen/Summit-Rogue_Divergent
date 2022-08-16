cmake_minimum_required(VERSION 3.7)
project(SDL2Test)

find_package(SDL2 REQUIRED)
include_directories(SDL2Test ${SDL2_INCLUDE_DIRS})

add_executable(SDL2Test 
main.cc
main.h)
target_link_libraries(SDL2Test ${SDL2_LIBRARIES})
