cmake_minimum_required(VERSION 3.16)

set(src ${CMAKE_CURRENT_SOURCE_DIR})
#link_directories(${TcInstallPrefix}/lib)
link_directories(/home/ws/cppserver/build)
link_directories(/home/ws/cppserver/build/modules)
link_directories(/home/ws/cppserver/build/modules/CppCommon)
link_directories(/home/ws/cppserver/build/modules/CppCommon/modules)

TcAddLibrary(
	TARGET tc-common
	SOURCE "${src}/common/*.cpp"
	INCLUDE "${TcIncludeDir}"
	VERSION ${TcVersion}
)

TcAddLibrary(
	TARGET tc-parser
	SOURCE "${src}/parser/*.cpp"
	INCLUDE "${TcIncludeDir}" "/home/ws/cppserver/modules/CppCommon/modules/fmt/include" "/home/ws/cppserver/modules/CppCommon/include"
	LIBRARIES
		tc-common
		fmt
	VERSION ${TcVersion}
)

TcAddLibrary(
	TARGET tc-server
	SOURCE "${src}/server/*.cpp"
	INCLUDE "${TcIncludeDir}" "/home/ws/cppserver/include" "/home/ws/cppserver/modules/asio/asio/include/asio"
	LIBRARIES
		asio
		cppserver
		cppcommon
		tc-parser
		spdlog
	VERSION ${TcVersion}
)

TcAddExecutable(tc-telematics-server "${src}/telematics-server.cpp" tc-server
	LINK_FLAGS "-Wl,--no-as-needed")