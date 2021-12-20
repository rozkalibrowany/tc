cmake_minimum_required(VERSION 3.1.0)

set(PROJECT "parser-test")
project(${PROJECT})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++20")

include_directories(parser)

set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/Packet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/common/Result.h ≤
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/reader/Reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/reader/Buf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/records/ParserI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/records/RecordI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/records/avl/AVLRecord.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/records/avl/AVLPacket.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/records/avl/AVLRecordReader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/records/avl/AVLRecordReaderParser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/records/gps/GPSRecord.h
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/records/gps/GPSParser.h
)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/Packet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/reader/Reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/reader/Buf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/records/avl/AVLRecord.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/parser/main.cpp
)


add_executable(${PROJECT} ${SRC})
