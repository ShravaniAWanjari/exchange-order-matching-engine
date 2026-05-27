cmake_minimum_required(VERSION 3.16)
project(exchange_order_match)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

add_executable(lob_engine src/main.cpp)