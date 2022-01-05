cmake_minimum_required(VERSION 3.16)

set(src ${CMAKE_CURRENT_SOURCE_DIR})

TcAddLibrary(
	TARGET tc-parser
	SOURCE "${src}/*.cpp"
	INCLUDE "${TcIncludeDir}"
	LIBRARIES fmt::lib
	VERSION ${TcVersion}
)

TcAddExecutable(tc-parser-test "test/parser-test.cpp" "tc-parser"
	LINK_FLAGS "-Wl,--no-as-needed"
)