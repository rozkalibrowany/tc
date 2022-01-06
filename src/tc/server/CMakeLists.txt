cmake_minimum_required(VERSION 3.16)

set(src ${CMAKE_CURRENT_SOURCE_DIR})

TcAddLibrary(
	TARGET tc-server
	SOURCE "${src}/tcp/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
		tc-parser
		asio
		cppcommon
		cppserver
		spdlog
	VERSION ${TcVersion}
)

TcAddExecutable(tc-tcp-server "tcp/tcp-server.cpp" "tc-server"
	LINK_FLAGS "-Wl,--no-as-needed"
)

#TcAddExecutable(tc-server-test "test/server-test.cpp" "tc-server"
#	LINK_FLAGS "-Wl,--no-as-needed"
#)