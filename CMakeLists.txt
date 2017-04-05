cmake_minimum_required(VERSION 2.8)

project( MOE )

option(BUILD_EXAMPLES "Builds examples" ON)
option(BUILD_TESTS "Builds Tests" OFF)
option(DEBUG "Enables debug symbols" OFF)


include_directories( "include" )

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    # using Clang

    add_compile_options(
                    -std=c++14
                    -Wall
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

if(BUILD_EXAMPLES)

    set(SOURCE
        "examples/hello_world.cpp"
        )

    add_executable(hello_world ${SOURCE})

    set(SOURCE
        "examples/square_root.cpp"
        )

    add_executable(square_root ${SOURCE})

    set(SOURCE
        "examples/salesman.cpp"
        )
    
    add_executable(salesman ${SOURCE})
endif()

if(BUILD_TESTS)
    set(SOURCE
        "tests/tests.cpp"
        )
        
    add_executable(tests ${SOURCE})
endif()
