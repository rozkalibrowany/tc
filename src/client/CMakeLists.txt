cmake_minimum_required(VERSION 3.16)

set(src ${CMAKE_CURRENT_SOURCE_DIR})

TcAddLibrary(
	TARGET tc-telematics-client
	SOURCE "*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
		tc-common
		cppcommon
		cppserver
		spdlog
	VERSION ${TcVersion}
)

TcAddExecutable(tc-telematics-client "telematics-client.cpp" "tc-telematics-client"
	LINK_FLAGS "-Wl,--no-as-needed"
)