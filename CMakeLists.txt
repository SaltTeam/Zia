cmake_minimum_required(VERSION 3.9)
project(zia)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CPP_FLAGS "${CMAKE_CPP_FLAGS} -W -Wall -Wextra -frtti")

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
        sza-plus-plus/api/pp/conf.cpp
        modules/MIME_types.cpp)

add_library(modStaticFiles SHARED ${modStaticFilesSourceFiles})

set(modPhpCgiSourceFiles
        modules/modPhpCgi/modPhpCgi.cpp
        modules/modPhpCgi/modPhpCgi.hpp
        sza-plus-plus/api/pp/conf.cpp
        core/sources/Utils.cpp core/includes/Utils.hpp)

add_library(modPhpCgi SHARED ${modPhpCgiSourceFiles})

set(modNetworkSourceFiles
        modules/modNetwork/ModNet.cpp
        modules/modNetwork/ModNet.hpp
        modules/modNetwork/ZiaSocket.hpp
        modules/modNetwork/lib/sources/Addr.cpp
        modules/modNetwork/lib/sources/Select.cpp
        modules/modNetwork/lib/sources/Socket.cpp
        modules/modNetwork/lib/sources/SocketException.cpp
        )

add_library(modNetwork SHARED ${modNetworkSourceFiles})

set(modSSLSourceFiles
        modules/modSSL/modSSL.cpp
        modules/modSSL/modSSL.hpp
        modules/modSSL/ZiaSocket.hpp
        modules/modSSL/Select.cpp
        modules/modSSL/Select.hpp
        )

add_library(modSSL SHARED ${modSSLSourceFiles})

if (WIN32)

else()
# Search OpenSSL
find_package(PkgConfig REQUIRED)
pkg_search_module(OPENSSL REQUIRED openssl)

if( OPENSSL_FOUND )
    include_directories(${OPENSSL_INCLUDE_DIRS})
    message(STATUS "Using OpenSSL ${OPENSSL_VERSION}")
else()
    # Error; with REQUIRED, pkg_search_module() will throw an error by it's own
endif()

target_link_libraries(modSSL ${OPENSSL_LIBRARIES})
endif()

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
    target_compile_options(modNetwork PRIVATE /std:c++latest)
    target_compile_options(modSSL PRIVATE /std:c++latest)
endif()
