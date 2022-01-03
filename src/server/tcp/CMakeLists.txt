cmake_minimum_required(VERSION 3.1.0)

set(PROJECT "server-tcp-test")
project(${PROJECT})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(.)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++20")

file(GLOB_RECURSE SRC ${CMAKE_CURRENT_SOURCE_DIR}/asio/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/handler/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
file(GLOB_RECURSE HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

add_executable(${PROJECT} ${SRC})

find_package(cppserver)
target_link_libraries(${PROJECT}  cppserver::cppserver)