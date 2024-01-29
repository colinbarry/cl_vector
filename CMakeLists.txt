cmake_minimum_required(VERSION 3.14)
project(cl_vector)

add_library(cl_vector INTERFACE cl_vector.h)
target_include_directories(cl_vector INTERFACE .)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)
endif()

if (BUILD_TESTING AND CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(tests)
endif()
