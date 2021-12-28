cmake_minimum_required(VERSION 3.1.0)

set(PROJECT "parser-test")
project(${PROJECT})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(.)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++20")

add_subdirectory(fmt)


set(HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/Packet.h
    ${CMAKE_CURRENT_SOURCE_DIR}/common/Result.h
    ${CMAKE_CURRENT_SOURCE_DIR}/reader/Reader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/reader/Buf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/records/ParserI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/records/RecordI.h
    ${CMAKE_CURRENT_SOURCE_DIR}/records/avl/AVLRecord.h
    ${CMAKE_CURRENT_SOURCE_DIR}/records/avl/AVLPacket.h
    ${CMAKE_CURRENT_SOURCE_DIR}/records/avl/AVLRecordHeader.h
    ${CMAKE_CURRENT_SOURCE_DIR}/records/io/IoRecord.h
    ${CMAKE_CURRENT_SOURCE_DIR}/records/gps/GPSRecord.h
)

set(SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/Packet.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/reader/Reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/reader/Buf.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/records/avl/AVLRecord.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/records/avl/AVLRecordHeader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/records/io/IoRecord.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/records/gps/GPSRecord.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)


add_executable(${PROJECT} ${SRC})

find_package(fmt)
target_link_libraries(${PROJECT}  fmt::fmt)