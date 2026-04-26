cmake_minimum_required(VERSION 3.16)
project(low_latency_lob_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

add_executable(lob_engine
    src/main.cpp
    src/order_book.cpp
)

add_executable(benchmark_engine
    benchmark/benchmark.cpp
    src/order_book.cpp
)
