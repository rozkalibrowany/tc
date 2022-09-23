cmake_minimum_required(VERSION 3.16)

set(src ${CMAKE_CURRENT_SOURCE_DIR})

TcAddLibrary(
	TARGET tc-common
	SOURCE "${src}/common/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
		spdlog::spdlog
	VERSION ${TcVersion}
)

TcAddLibrary(
	TARGET tc-parser
	SOURCE "${src}/parser/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
		jsoncpp
	VERSION ${TcVersion}
)

TcAddLibrary(
	TARGET tc-iot
	SOURCE "${src}/server/iot/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
	 tc-parser
	 mongocxx
	 bsoncxx
	VERSION ${TcVersion}
)

TcAddLibrary(
	TARGET tc-asio
	SOURCE "${src}/asio/*.cpp"
	INCLUDE "${TcIncludeDir}" "/usr/local/include"
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
		tc-iot
		tc-parser
		tc-common
		cppserver
		cppcommon
	VERSION ${TcVersion}
)

TcAddExecutable(tc-telematics-server "${src}/telematics-server.cpp" tc-server
	LINK_FLAGS "-Wl,--no-as-needed")

TcAddLibrary(
	TARGET tc-client
	SOURCE "${src}/client/tcp/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
		args-parser::lib
		tc-asio
		tc-common
		cppserver
		cppcommon
		tc-iot
	VERSION ${TcVersion}
)

TcAddExecutable(tc-telematics-client "${src}/telematics-client.cpp" tc-client
	LINK_FLAGS "-Wl,--no-as-needed")


TcAddLibrary(
	TARGET tc-vehicle-connector
	SOURCE "${src}/server/http/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
	tc-asio
	tc-common
	cppserver
	cppcommon
	tc-iot
	VERSION ${TcVersion}
)

TcAddExecutable(tc-vehicle-connector-server "${src}/vehicle-connector-server.cpp" tc-vehicle-connector
	LINK_FLAGS "-Wl,--no-as-needed")
