cmake_minimum_required(VERSION 3.16)
project(cmake_wrapper)

set(CMAKE_VERBOSE_MAKEFILE 1)
include(conanbuildinfo.cmake)
conan_basic_setup()

if (NOT BUILD_SYSTEM)
	add_subdirectory(source_subfolder)
endif()

install(
	DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/source_subfolder/include"
  DESTINATION "${CMAKE_INSTALL_PREFIX}/include"
  FILES_MATCHING
  PATTERN "*.h"
)