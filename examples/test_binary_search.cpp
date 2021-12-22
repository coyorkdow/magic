//
// Created by YorkDow Co on 2021/12/22.
//

#include "test_binary_search.h"

#include <algorithm>

using namespace magic;

void TestBinarySearch() {
  constexpr int arr[] = {1, 2, 3, 4, 4, 5, 5, 5, 6, 7};
  static_assert(BinarySearch<int>::LowerBound(arr, 4) == 3, "");
  static_assert(BinarySearch<int>::UpperBound(arr, 4) == 5, "");
  static_assert(BinarySearch<int>::LowerBound(arr, 5) == 5, "");
  static_assert(BinarySearch<int>::UpperBound(arr, 5) == 8, "");
  static_assert(BinarySearch<int>::LowerBound(arr, 7) == 9, "");
  static_assert(BinarySearch<int>::UpperBound(arr, 7) == 10, "");
  static_assert(BinarySearch<int>::LowerBound(arr, 8) == 10, "");
  static_assert(BinarySearch<int>::UpperBound(arr, 8) == 10, "");
  static_assert(BinarySearch<int>::LowerBound(arr, 1) == 0, "");
  static_assert(BinarySearch<int>::UpperBound(arr, 1) == 1, "");
  static_assert(BinarySearch<int>::LowerBound(arr, 0) == 0, "");
  static_assert(BinarySearch<int>::UpperBound(arr, 0) == 0, "");

  using std::begin;
  using std::end;
#define all(x) begin(x), end(x)
  assert(BinarySearch<int>::LowerBound(arr, 4) == std::lower_bound(all(arr), 4) - begin(arr));
  assert(BinarySearch<int>::UpperBound(arr, 4) == std::upper_bound(all(arr), 4) - begin(arr));
  assert(BinarySearch<int>::LowerBound(arr, 5) == std::lower_bound(all(arr), 5) - begin(arr));
  assert(BinarySearch<int>::UpperBound(arr, 5) == std::upper_bound(all(arr), 5) - begin(arr));
  assert(BinarySearch<int>::LowerBound(arr, 7) == std::lower_bound(all(arr), 7) - begin(arr));
  assert(BinarySearch<int>::UpperBound(arr, 7) == std::upper_bound(all(arr), 7) - begin(arr));
  assert(BinarySearch<int>::LowerBound(arr, 8) == std::lower_bound(all(arr), 8) - begin(arr));
  assert(BinarySearch<int>::UpperBound(arr, 8) == std::upper_bound(all(arr), 8) - begin(arr));
  assert(BinarySearch<int>::LowerBound(arr, 1) == std::lower_bound(all(arr), 1) - begin(arr));
  assert(BinarySearch<int>::UpperBound(arr, 1) == std::upper_bound(all(arr), 1) - begin(arr));
  assert(BinarySearch<int>::LowerBound(arr, 0) == std::lower_bound(all(arr), 0) - begin(arr));
  assert(BinarySearch<int>::UpperBound(arr, 0) == std::upper_bound(all(arr), 0) - begin(arr));

  constexpr double float_arr[] = {1.1, 2.2, 3.2, 4.5, 4.56, 5.6, 7};
  static_assert(BinarySearch<double>::LowerBound(float_arr, 5.6) == 5, "");
  assert(BinarySearch<double>::LowerBound(float_arr, 5.6)
         == std::lower_bound(all(float_arr), 5.6) - begin(float_arr));
}
