cmake_minimum_required(VERSION 3.15)
project(ExchangeOrderMatch CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/O2 /Oi /Ot /std:c++20)
else()
    add_compile_options(-O3 -march=native -Wall - Wextra)
endif()

add_executable(matching_engine src/main.cpp)

target_include_directories(matching_engine PRIVATE src)