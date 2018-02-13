cmake_minimum_required(VERSION 3.9)
project(zia)

set(CMAKE_CXX_STANDARD 17)

include_directories(inc)

add_executable(zia main.cpp inc/Library.hpp src/Library.cpp)
