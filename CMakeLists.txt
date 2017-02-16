cmake_minimum_required(VERSION 2.8)

project( MOE )

option(BUILD_SHARED "ON to build MOE as SHARED, OFF to build MOE AS STATIC" OFF)
option(BUILD_EXAMPLES "ON to build examples" ON)
option(DEBUG "Enable debugging symbols" OFF)

include_directories( "include" )

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    # using Clang

    add_compile_options(
                    -std=c++14
                    -Wall
                    -Wextra
                    -pedantic
                    )
    if(DEBUG)
        #Debug Mode
        add_compile_options(
                    -g
                    )
    else()
        #Release Mode
        add_compile_options(
                    -O3
                    )
    endif()
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    
    # using GCC

    add_compile_options(
                    -std=c++14
                    -Wall
                    -Wextra
                    -pedantic
                    )
    if(DEBUG)
        #Debug Mode
        add_compile_options(
                    -g
                    )
    else()
        #Release Mode
        add_compile_options(
                    -O3
                    )
    endif()

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")

    # using Visual Studio C++

    add_compile_options(/std=c++14
                    /W4
                    /Wall
                    )
    if(DEBUG)
        #Debug Mode
    else()
        #Release Mode
        add_compile_options( /O2t )
    endif()
endif()


set(SOURCES
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

    set(SOURCE
        "examples/salesman.cpp"
        )
    
    add_executable(salesman ${SOURCE})
    target_link_libraries(salesman ${LINK_LIBS})
endif()