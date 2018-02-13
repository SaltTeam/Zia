cmake_minimum_required(VERSION 3.9)
project(zia)

set(CMAKE_CXX_STANDARD 17)

include_directories(sza-plus-plus/api/pp core/includes)

add_executable(zia core/sources/main.cpp

        sza-plus-plus/api/pp/conf.cpp sza-plus-plus/api/pp/conf.hpp sza-plus-plus/api/pp/module.hpp sza-plus-plus/api/pp/http.hpp sza-plus-plus/api/pp/net.hpp
        core/includes/network/Addr.hpp core/sources/network/Addr.cpp
        core/includes/network/Select.hpp core/sources/network/Select.cpp
        core/includes/network/Socket.hpp core/sources/network/Socket.cpp
        core/includes/network/SocketException.hpp core/sources/network/SocketException.cpp
        core/includes/Usings.hpp
        core/sources/Core.cpp core/includes/Core.hpp
        core/sources/Pipeline.cpp core/includes/Pipeline.hpp)

if (WIN32)
    target_compile_options(zia PRIVATE /std:c++latest)
endif()