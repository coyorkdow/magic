#include <iostream>

#include "quick_power.h"
#include "binary_search.h"

int main() {
  constexpr int arr[] = {1, 2, 3, 4, 4, 5, 5, 5, 6, 7};
  char show0[Power<2, 10, 998244353>::Calculate()];
  char show1[BinarySearch<int>::LowerBound(arr, 4)];
  char show2[BinarySearch<int>::UpperBound(arr, 4)];
  static_assert(sizeof(show0) == 1024, "");
  static_assert(sizeof(show1) == 3, "");
  static_assert(sizeof(show2) == 5, "");
  std::cout << sizeof(show0) << ' ' << sizeof(show1) << ' ' << sizeof(show2) << std::endl;
  std::cout << BinarySearch<int>::LowerBound(arr, 5) // expected 5
            << ' ' << BinarySearch<int>::UpperBound(arr, 5) // expected 8
            << ' ' << BinarySearch<int>::LowerBound(arr, 7) // expected 9
            << ' ' << BinarySearch<int>::UpperBound(arr, 7) // expected 10
            << ' ' << BinarySearch<int>::LowerBound(arr, 8) // expected 10
            << ' ' << BinarySearch<int>::UpperBound(arr, 8) // expected 10
            << ' ' << BinarySearch<int>::LowerBound(arr, 1) // expected 0
            << ' ' << BinarySearch<int>::UpperBound(arr, 1) // expected 1
            << ' ' << BinarySearch<int>::LowerBound(arr, 0) // expected 0
            << ' ' << BinarySearch<int>::UpperBound(arr, 0); // expected 0
}