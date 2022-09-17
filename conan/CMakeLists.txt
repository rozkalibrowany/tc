cmake_minimum_required(VERSION 3.16)

project(tc-conan CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../tool/cmake")
include(tc)

tc_detect_system()
tc_set_colors(ON)

TcOptMsg("TC_SYSTEM_NAME"               TC_SYSTEM_NAME)
TcOptMsg("TC_SYSTEM_VERSION"            TC_SYSTEM_VERSION)
TcOptMsg("CMAKE_BUILD_TYPE"             CMAKE_BUILD_TYPE)

TcOptMsg("CMAKE_C_COMPILER"             CMAKE_C_COMPILER)
TcOptMsg("CMAKE_C_COMPILER_ID"          CMAKE_C_COMPILER_ID)
TcOptMsg("CMAKE_C_COMPILER_VERSION"     CMAKE_C_COMPILER_VERSION)
TcOptMsg("CMAKE_C_STANDARD"             CMAKE_C_STANDARD)

TcOptMsg("CMAKE_CXX_COMPILER"           CMAKE_CXX_COMPILER)
TcOptMsg("CMAKE_CXX_COMPILER_ID"        CMAKE_CXX_COMPILER_ID)
TcOptMsg("CMAKE_CXX_COMPILER_VERSION"   CMAKE_CXX_COMPILER_VERSION)
TcOptMsg("CMAKE_CXX_STANDARD"           CMAKE_CXX_STANDARD)


add_custom_target(conan-build)
add_custom_target(conan-build-ubuntu-dev)
add_custom_target(conan-upload)
add_custom_target(conan-remove)
add_custom_target(conan-remove-force)

add_custom_target(
	conan-list-local
	COMMAND conan search
)

add_custom_target(
	conan-remove-local-all
	COMMAND conan search
	COMMAND conan remove \"*\" -f
	COMMAND conan search
)

add_custom_target(
	conan-list-remote
	COMMAND conan search -r rg-oss
)

add_custom_target(
	conan-remove-remote-all
	COMMAND conan search
	COMMAND conan remove \"*\" -f -r rg-oss
	COMMAND conan search
)

add_custom_target(
	conan-profiles
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/settings.yml ~/.conan/
	COMMAND ${CMAKE_COMMAND} -E make_directory ~/.conan/profiles
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/profiles/default ~/.conan/profiles/
)

tc_conan_build_package(NAME tc
	VERSION 0.3.0
	DEPENDS conan-profiles
	UBUNTU
)

tc_conan_build_package(NAME asio
	VERSION 1.22.1
	DEPENDS conan-profiles conan-tc-build
	UBUNTU
)

tc_conan_build_package(NAME args-parser
	VERSION 6.2.0.1
	DEPENDS conan-profiles conan-tc-build
	UBUNTU
)

tc_conan_build_package(NAME cppserver
	VERSION master
	DEPENDS conan-profiles conan-tc-build
	UBUNTU
)

tc_conan_build_package(NAME jsoncpp
	VERSION 1.9.5
	DEPENDS conan-profiles conan-tc-build
	UBUNTU
)

tc_conan_build_package(NAME mongo-c-driver
	VERSION 1.22.0
	DEPENDS conan-profiles conan-tc-build
	UBUNTU
)


tc_conan_build_package(NAME mongo-cxx-driver
	VERSION r3.6.7
	DEPENDS conan-profiles conan-tc-build
	UBUNTU
)

#tc_conan_build_package(NAME spdlog
#	VERSION v1.x
#	DEPENDS conan-profiles conan-tc-build
#	UBUNTU
#)

#tc_conan_build_package(NAME fmt
#	VERSION 9.1.0
#	DEPENDS conan-profiles conan-tc-build
#	UBUNTU
#)

tc_conan_build_package(NAME vscode
	VERSION 1.0.2
	DEPENDS conan-profiles conan-tc-build
	UBUNTU
)

message("${TcCbr}set ${TcCby}CONAN_CPU_COUNT${TcCra} for cpu count")