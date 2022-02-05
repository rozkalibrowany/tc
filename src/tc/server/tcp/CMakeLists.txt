cmake_minimum_required(VERSION 3.16)

set(src ${CMAKE_CURRENT_SOURCE_DIR})

TcAddLibrary(
	TARGET tc-telematics-server
	SOURCE "*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
		tc-parser
		cppcommon
		cppserver
		asio
		spdlog
	VERSION ${TcVersion}
)

TcAddExecutable(tc-telematics-server "telematics-server.cpp" "tc-telematics-server"
	LINK_FLAGS "-Wl,--no-as-needed"
)