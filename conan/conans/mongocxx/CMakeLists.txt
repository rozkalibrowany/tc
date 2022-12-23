cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project(cmake_wrapper)
find_package(mongoc REQUIRED)

include(conanbuildinfo.cmake)
conan_basic_setup()

if (NOT BUILD_SYSTEM)
	add_subdirectory(source_subfolder)
endif()