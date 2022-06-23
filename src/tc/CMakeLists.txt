cmake_minimum_required(VERSION 3.16)

set(src ${CMAKE_CURRENT_SOURCE_DIR})

TcAddLibrary(
	TARGET tc-common
	SOURCE "${src}/common/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
		fmt
	VERSION ${TcVersion}
)

TcAddLibrary(
	TARGET tc-parser
	SOURCE "${src}/parser/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
		jsoncpp
		tc-common
	VERSION ${TcVersion}
)

TcAddLibrary(
	TARGET tc-iot
	SOURCE "${src}/server/iot/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
	 tc-parser
	VERSION ${TcVersion}
)

TcAddLibrary(
	TARGET tc-asio
	SOURCE "${src}/asio/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
		asio
	VERSION ${TcVersion}
)

TcAddLibrary(
	TARGET tc-server
	SOURCE "${src}/server/tcp/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
		tc-asio
		cppserver
		cppcommon
		tc-iot
		tc-parser
		spdlog
	VERSION ${TcVersion}
)

TcAddExecutable(tc-telematics-server "${src}/telematics-server.cpp" tc-server
	LINK_FLAGS "-Wl,--no-as-needed")

TcAddLibrary(
	TARGET tc-client
	SOURCE "${src}/client/tcp/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
	tc-asio
	cppserver
	cppcommon
	tc-iot
	spdlog
	VERSION ${TcVersion}
)

TcAddExecutable(tc-telematics-client "${src}/telematics-client.cpp" tc-client
	LINK_FLAGS "-Wl,--no-as-needed")


TcAddLibrary(
	TARGET tc-vehicle-connector
	SOURCE "${src}/server/http/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
	tc-client
	tc-asio
	cppserver
	cppcommon
	tc-iot
	spdlog
	VERSION ${TcVersion}
)

TcAddExecutable(tc-vehicle-connector-server "${src}/vehicle-connector-server.cpp" tc-vehicle-connector
	LINK_FLAGS "-Wl,--no-as-needed")
