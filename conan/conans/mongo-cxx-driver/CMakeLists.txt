cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project(cmake_wrapper)

set(CMAKE_VERBOSE_MAKEFILE 1)
set(BUILD_VERSION 1.19.2)
include(conanbuildinfo.cmake)
conan_basic_setup()

if (NOT BUILD_SYSTEM)
	add_subdirectory(source_subfolder)
endif()

install(
	CODE
		"
		message(STATUS \"install dummy\")
		"
)