cmake_minimum_required(VERSION 3.1.0)

set(PROJECT "parser-test")
project(${PROJECT})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(.)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++20")

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/common/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/reader/*.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/records/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/Packet.cpp ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(${PROJECT} ${SRC})

find_package(fmt)
target_link_libraries(${PROJECT}  fmt::fmt)