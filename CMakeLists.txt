cmake_minimum_required(VERSION 3.13)
project(magic)

set(CMAKE_CXX_STANDARD 11)

include_directories(./src)

add_compile_options(-Wall)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(install_gtest OFF)
set(install_gmock OFF)
set(build_gmock ON)

add_subdirectory(./third_party/googletest)

add_executable(magic
        src/util.h
        src/quick_power.h
        src/binary_search.h
        src/index_sequence.h
        src/tuple.h
        src/reflection.h
        src/type_of.h
        src/traits.h
        src/currying.h
        examples/main.cpp)

target_compile_options(magic PUBLIC -g)

target_link_libraries(magic gmock gtest gtest_main)

add_test(NAME magic COMMAND magic)