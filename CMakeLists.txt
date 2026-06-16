cmake_minimum_required(VERSION 3.15)
project(lob_engine CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(
        -O3
        -march=native
        -flto
        -Wall -Wextra
        -Werror
    )
    add_link_options(-flto)
endif()