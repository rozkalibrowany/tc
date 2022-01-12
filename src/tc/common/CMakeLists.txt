cmake_minimum_required(VERSION 3.16)

set(src ${CMAKE_CURRENT_SOURCE_DIR})

TcAddLibrary(
	TARGET tc-common
	SOURCE "${src}/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES
		rt
	VERSION ${TcVersion}
)