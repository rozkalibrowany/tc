cmake_minimum_required(VERSION 3.16)
project(test_package CXX)

set(CMAKE_VERBOSE_MAKEFILE 1)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

find_package(fmt REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} test_package.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} fmt::lib)