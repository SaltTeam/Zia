cmake_minimum_required(VERSION 3.9)
project(zia)

set(CMAKE_CXX_STANDARD 17)

add_executable(zia
        src/Library.cpp
        inc/Library.hpp)

include_directories(inc)