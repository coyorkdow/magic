cmake_minimum_required(VERSION 3.19)
project(magic)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_executable(magic
        main.cpp
        util.h
        quick_power.h
        binary_search.h
        index_sequence.h
        tuple.h)