cmake_minimum_required(VERSION 3.16)
set(ENV{VERBOSE} 1)

set_property(GLOBAL PROPERTY RULE_MESSAGES 1)
set_property(GLOBAL PROPERTY TARGET_MESSAGES 1)
set(CMAKE_COLOR_MAKEFILE   ON  CACHE BOOL "" FORCE)
set(CMAKE_VERBOSE_MAKEFILE OFF CACHE BOOL "" FORCE)
set(RULE_MESSAGES          ON  CACHE BOOL "" FORCE)
set(PROJECT_VERSION        0.1.0)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/tool/cmake")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug" CACHE PATH "CMAKE_BUILD_TYPE" FORCE)
endif()

project(TelematicsConnector LANGUAGES C CXX VERSION "${PROJECT_VERSION}")

include(ExternalProject)
include(FetchContent)
include(tc)

tc_detect_system()
tc_set_colors(ON)
tc_conan_get()

include(${CMAKE_BINARY_DIR}/conan.cmake)
include(GNUInstallDirs)

set(TC_CONANFILE conanfile.py)
tc_conan_cmake_run(
	CONANFILE ${TC_CONANFILE}
	BASIC_SETUP
	CMAKE_TARGETS
	BUILD tc
)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

enable_testing()

set_property(GLOBAL PROPERTY TC_VERSION "${PROJECT_VERSION}")
set_property(GLOBAL PROPERTY CMAKE_POSITION_INDEPENDENT_CODE CACHE ON)
set_property(GLOBAL PROPERTY CMAKE_CXX_FLAGS "" CACHE ON)
set_property(GLOBAL PROPERTY CMAKE_CXX_RELEASE_FLAGS "" CACHE ON)
set_property(GLOBAL PROPERTY CMAKE_CXX_DEBUG_FLAGS "" CACHE ON)

set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib") # static libs
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/lib") # shared libs
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin") # binaries

set(CMAKE_CXX_FLAGS_BASE "-std=c++2a -D_FILE_OFFSET_BITS=64 -D_REENTRANT -fPIC -fno-strict-aliasing -fno-stack-protector -fdiagnostics-color=always -Wno-unused-parameter -Wno-unknown-pragmas -fdiagnostics-color=always")

set(CMAKE_CXX_FLAGS_DEBUG_EXTERNAL      "${CMAKE_CXX_FLAGS_RELEASE} -O1")
set(CMAKE_CXX_FLAGS_DEBUG_EXTERNAL_17   "${CMAKE_CXX_FLAGS_RELEASE} -O1")
set(CMAKE_CXX_FLAGS_RELEASE_EXTERNAL    "${CMAKE_CXX_FLAGS_RELEASE} -O2")
set(CMAKE_CXX_FLAGS_RELEASE_EXTERNAL_17 "${CMAKE_CXX_FLAGS_RELEASE} -O2 -std=c++2a")

set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}   -Wall -O0 -DTRC -ggdb3") # -Werror
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -Wno-unused-result -O2 -g") #- Werror

message("")

SET(TcVersion  "${TC_VERSION}" CACHE STRING "TcVersion" FORCE)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_BASE}")
SET(TcCompileFlags  "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_${CMAKE_BUILD_TYPE}}" CACHE STRING "TcCompileFlags" FORCE) # RIGHT BRACKET
string (REPLACE " " ";" TcCompileFlagsList "${TcCompileFlags}")
SET(TcSourceDir     "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING "TcSourceDir"     FORCE)
SET(TcCmakeDir      "${TcSourceDir}/tool/cmake"   CACHE STRING "TcCmakeDir"      FORCE)
SET(TcIncludeDir    "${TcSourceDir}/src"          CACHE STRING "TcIncludeDir"    FORCE)
SET(TcInstallPrefix "/usr/local/ws"               CACHE STRING "TcInstallPrefix" FORCE)

set(CMAKE_INSTALL_PREFIX "${TcInstallPrefix}" CACHE PATH "Prefix prepended to install directories" FORCE)

TcOptMsg("CMAKE_PROJECT_NAME"         CMAKE_PROJECT_NAME)
TcOptMsg("CMAKE_VERSION"              CMAKE_VERSION)
TcOptMsg("CMAKE_INSTALL_PREFIX"       CMAKE_INSTALL_PREFIX)
TcOptMsg("CMAKE_CURRENT_BINARY_DIR"   CMAKE_CURRENT_BINARY_DIR)
TcOptMsg("CMAKE_CURRENT_SOURCE_DIR"   CMAKE_CURRENT_SOURCE_DIR)

TcOptMsg("CMAKE_BUILD_TYPE"           CMAKE_BUILD_TYPE)

TcOptMsg("CMAKE_C_COMPILER"           CMAKE_C_COMPILER)
TcOptMsg("CMAKE_C_COMPILER_ID"        CMAKE_C_COMPILER_ID)
TcOptMsg("CMAKE_C_COMPILER_VERSION"   CMAKE_C_COMPILER_VERSION)
TcOptMsg("CMAKE_C_STANDARD"           CMAKE_C_STANDARD)

TcOptMsg("CMAKE_CXX_COMPILER"         CMAKE_CXX_COMPILER)
TcOptMsg("CMAKE_CXX_COMPILER_ID"      CMAKE_CXX_COMPILER_ID)
TcOptMsg("CMAKE_CXX_COMPILER_VERSION" CMAKE_CXX_COMPILER_VERSION)
TcOptMsg("CMAKE_CXX_STANDARD"         CMAKE_CXX_STANDARD)

TcOptMsg("CXX"                        CMAKE_CXX_COMPILER)
TcOptMsg("CXX_FLAGS"                  CMAKE_CXX_FLAGS)
TcOptMsg("CXX_FLAGS_DEBUG"            CMAKE_CXX_FLAGS_DEBUG)
TcOptMsg("CXX_FLAGS_RELEASE"          CMAKE_CXX_FLAGS_RELEASE)
TcOptMsg("CXX_FLAGS_DEBUG_EXTERNAL"   CMAKE_CXX_FLAGS_DEBUG_EXTERNAL)
TcOptMsg("CXX_FLAGS_RELEASE_EXTERNAL" CMAKE_CXX_FLAGS_RELEASE_EXTERNAL)
TcOptMsg("CompileFlags"               TcCompileFlags)

TcOptMsg("TC_CONANFILE"               TC_CONANFILE)
TcOptMsg("TC_SYSTEM_NAME"             TC_SYSTEM_NAME)
TcOptMsg("TC_SYSTEM_VERSION"          RG_SYSTEM_VERSION)
message(STATUS "")

tc_find_package_status(NAME cppserver REQUIRED)
tc_find_package_status(NAME fmt REQUIRED)
tc_find_package_status(NAME jsoncpp REQUIRED)
#tc_find_package_status(NAME cppcommon REQUIRED)
tc_find_package_status(NAME spdlog REQUIRED)
tc_find_package_status(NAME mongoc REQUIRED)
tc_find_package_status(NAME mongocxx REQUIRED)

add_subdirectory("src/tc")

add_custom_target(
	conan-install
	COMMAND conan imports ${CMAKE_CURRENT_SOURCE_DIR}/conanfile.py -imf ${CMAKE_INSTALL_PREFIX}
)