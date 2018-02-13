cmake_minimum_required(VERSION 3.9)
project(zia)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -W -Wall -Wextra")

include_directories(.)

add_executable(zia core/sources/main.cpp)

set(modExampleSourceFiles
        modules/modExample/modExample.cpp
        modules/modExample/modExample.hpp)

add_library(modExample SHARED ${modExampleSourceFiles})

if (WIN32)
    target_compile_options(zia PRIVATE /std:c++latest)
    target_compile_options(modExample PRIVATE /std:c++latest)
endif()