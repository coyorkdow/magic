cmake_minimum_required(VERSION 3.13)
project(magic)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)

add_compile_options(-Wall)

add_executable(magic
        util.h
        quick_power.h
        binary_search.h
        index_sequence.h
        tuple.h
        reflection.h
        type_of.h
        traits.h

        examples/main.cpp
        examples/test_reflection.cpp
        examples/test_reflection.h
        examples/examples_util.h
        examples/test_name_of.cpp
        examples/test_name_of.h
        examples/test_quick_power.cpp
        examples/test_quick_power.h
        examples/test_binary_search.cpp
        examples/test_binary_search.h
        examples/test_tuple.cpp
        examples/test_tuple.cpp
        examples/test_tuple.h)
