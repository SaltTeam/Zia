cmake_minimum_required(VERSION 3.9)
project(zia)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -W -Wall -Wextra")

include_directories(
        sza-plus-plus/api/pp
        core/includes
        modules/
        nlohmann/
        .
)

set(API_SOURCES
        sza-plus-plus/api/pp/conf.cpp
        sza-plus-plus/api/pp/conf.hpp
        sza-plus-plus/api/pp/module.hpp
        sza-plus-plus/api/pp/http.hpp
        sza-plus-plus/api/pp/net.hpp)


add_executable(zia core/sources/main.cpp
        ${API_SOURCES}

        core/sources/main.cpp

        sza-plus-plus/api/pp/conf.cpp sza-plus-plus/api/pp/conf.hpp sza-plus-plus/api/pp/module.hpp sza-plus-plus/api/pp/http.hpp sza-plus-plus/api/pp/net.hpp
        core/includes/network/Addr.hpp core/sources/network/Addr.cpp
        core/includes/network/Select.hpp core/sources/network/Select.cpp
        core/includes/network/Socket.hpp core/sources/network/Socket.cpp
        core/includes/network/SocketException.hpp core/sources/network/SocketException.cpp
        core/includes/ModNet.hpp core/sources/ModNet.cpp
        core/includes/ZiaSocket.hpp
        core/includes/Usings.hpp
        core/sources/Library.cpp
        core/includes/Library.hpp
        core/sources/Core.cpp core/includes/Core.hpp
        core/sources/Pipeline.cpp core/includes/Pipeline.hpp
        core/sources/ParserConfig.cpp core/includes/ParserConfig.hpp
        core/sources/Processing.cpp core/includes/Processing.hpp
        core/sources/Utils.cpp core/includes/Utils.hpp)

if (WIN32)
    find_package(Threads REQUIRED)
    target_link_libraries(zia ${CMAKE_THREAD_LIBS_INIT})
else ()
    target_link_libraries(zia pthread dl)
endif ()

#######################
#       Modules       #
#######################

set(modExampleSourceFiles
        modules/modExample/modExample.cpp
        modules/modExample/modExample.hpp
        sza-plus-plus/api/pp/conf.cpp)

add_library(modExample SHARED ${modExampleSourceFiles})

set(modStaticFilesSourceFiles
        modules/modStaticFiles/modStaticFiles.cpp
        modules/modStaticFiles/modStaticFiles.hpp
        modules/MIME_types.cpp)

add_library(modStaticFiles SHARED ${modStaticFilesSourceFiles})

set(modPhpCgiSourceFiles
        modules/modPhpCgi/modPhpCgi.cpp
        modules/modPhpCgi/modPhpCgi.h)

add_library(modPhpCgi SHARED ${modPhpCgiSourceFiles})

#######################
#       !Modules      #
#######################

#######################
#        Test         #
#######################

if (NOT "$ENV{TRAVIS}" STREQUAL "")

    add_executable(test_modStaticFiles
            ${API_SOURCES}
            travis/test_modStaticFiles.cpp
            modules/modStaticFiles/modStaticFiles.cpp
            modules/MIME_types.cpp
    )

endif()

#######################
#        !Test        #
#######################

if (WIN32)
    target_compile_options(zia PRIVATE /std:c++latest)
    target_compile_options(modExample PRIVATE /std:c++latest)
    target_compile_options(modStaticFiles PRIVATE /std:c++latest)
endif()