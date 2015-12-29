cmake_minimum_required(VERSION 3.2.1)
project(Snake)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(Snake ${SOURCE_FILES})