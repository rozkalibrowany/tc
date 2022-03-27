cmake_minimum_required(VERSION 3.16)

set(src ${CMAKE_CURRENT_SOURCE_DIR})
link_directories(/home/ws/cppserver/build)
link_directories(/home/ws/cppserver/build/modules)
link_directories(/home/ws/cppserver/build/modules/CppCommon)

TcAddLibrary(
	TARGET tc-telematics-client
	SOURCE "${src}/tcp/*.cpp"
	INCLUDE "${TcIncludeDir}" "/home/ws/cppserver/include" "/home/ws/cppserver/modules/asio/asio/include/asio" "/home/ws/cppserver/modules/CppCommon/include"
	LIBRARIES
		tc-common
		asio
		cppcommon
		cppserver
		spdlog
	VERSION ${TcVersion}
)

TcAddExecutable(tc-telematics-client "telematics-client.cpp" "tc-telematics-client"
	LINK_FLAGS "-Wl,--no-as-needed"
)