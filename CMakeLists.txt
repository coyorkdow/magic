cmake_minimum_required(VERSION 3.19)
project(magic)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_compile_options(-Wall)

add_executable(magic
        main.cpp
        util.h
        quick_power.h
        binary_search.h
        index_sequence.h
        tuple.h
        reflection.h name_of.h)