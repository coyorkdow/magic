# magic
Fantasic magic of modern c++

This reposity is aimed to show the power of modern c++ features. All codes are compliant the standard of C++11. I would say sorry if you are a favor of C++14/C++17/C++20. LOL.

Usage can be found in `main.cpp`.

`quick_power.h`

Calculate the exponent in compile time with a time complexity of `O(log n)`,

`binary_search.h`

Binary searching in compile time.

`index_sequence.h`

Generate a compile time integer sequence, as well as `std::index_sequence` in C++14.

`tuple.h`

A simple implementation of tuple, rely on `index_sequence.h`.

`reflection.h`

A simple implementation of static reflection, rely on `tuple.h`.
