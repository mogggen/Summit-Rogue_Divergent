cmake_minimum_required(VERSION 3.7)
project(SDL2Test)

set(CMAKE_BUILD_TYPE Debug)

if (UNIX AND NOT APPLE)
    add_executable(SDL2Test
    main.cc
    consoleMain.h
    main.h
    hardcodedRaster.h)

    find_package(SDL2 REQUIRED)
    include_directories(SDL2Test ${SDL2_INCLUDE_DIRS})
    target_link_libraries(SDL2Test ${SDL2_LIBRARIES})
endif (UNIX AND NOT APPLE)

if (WIN32)
add_executable(SDL2Test
main.cc
consoleMain.h)
endif(WIN32)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SDL2Test)

