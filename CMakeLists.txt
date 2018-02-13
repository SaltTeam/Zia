cmake_minimum_required(VERSION 3.9)
project(zia)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -W -Wall -Wextra")

include_directories(. sza-plus-plus/api/pp core/includes)

add_executable(zia
        core/sources/main.cpp
        sza-plus-plus/api/pp/conf.cpp sza-plus-plus/api/pp/conf.hpp sza-plus-plus/api/pp/module.hpp sza-plus-plus/api/pp/http.hpp sza-plus-plus/api/pp/net.hpp

        core/includes/Usings.hpp
        core/sources/Library.cpp
        core/includes/Library.hpp
        core/sources/Core.cpp core/includes/Core.hpp
        core/sources/Pipeline.cpp core/includes/Pipeline.hpp)

set(modExampleSourceFiles
        modules/modExample/modExample.cpp
        modules/modExample/modExample.hpp)

add_library(modExample SHARED ${modExampleSourceFiles})

if (WIN32)
    target_compile_options(zia PRIVATE /std:c++latest)
    target_compile_options(modExample PRIVATE /std:c++latest)
endif()