cmake_minimum_required(VERSION 3.2)

project( MOE )

option(BUILD_SHARED "ON to build MOE as SHARED, OFF to build MOE AS STATIC" OFF)
option(BUILD_EXAMPLES "ON to build examples" ON)

include_directories( "include" )

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # using Clang
    if(DEBUG)
        #Debug Mode
        add_compile_options(-std=c++14
                    -Wall
                    -Wextra
                    -pedantic
                    -g
                    )
    else()
        #Release Mode
        add_compile_options(-std=c++14
                    -Wall
                    -Wextra
                    -pedantic
                    -O3
                    )
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    # using GCC
    if(DEBUG)
        #Debug Mode
        add_compile_options(-std=c++14
                    -Wall
                    -Wextra
                    -pedantic
                    -g
                    )
    else()
        #Release Mode
        add_compile_options(-std=c++14
                    -Wall
                    -Wextra
                    -pedantic
                    -O3
                    )
    endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    # using Visual Studio C++
    if(DEBUG)
        #Debug Mode
        add_compile_options(/std=c++14
                    /W4
                    /Wall
                    )
    else()
        #Release Mode
        add_compile_options(/std=c++14
                    /W4
                    /Wall
                    )
    endif()
endif()


set(SOURCES
    "include/moe/base/Moe.hpp"
    "include/moe/base/Moether.hpp"
    "include/moe/base/Moether_defs.hpp"
    "include/moe/moe.hpp"
    "src/Moe.cpp"
    )

if(BUILD_SHARED)
    add_library(MOE SHARED ${SOURCES})
else()
    add_library(MOE STATIC ${SOURCES})
endif()

if(BUILD_EXAMPLES)

    set(LINK_LIBS ${LINK_LIBS} MOE)

    set(SOURCE
        "examples/hello_world.cpp"
        )

    add_executable(hello_world ${SOURCE})
    target_link_libraries(hello_world ${LINK_LIBS})

    set(SOURCE
        "examples/square_root.cpp"
        )

    add_executable(square_root ${SOURCE})
    target_link_libraries(square_root ${LINK_LIBS})
endif()