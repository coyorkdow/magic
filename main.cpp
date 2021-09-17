#include <iostream>

#include "binary_search.h"
#include "index_sequence.h"
#include "quick_power.h"
#include "tuple.h"

using namespace magic;

// https://stackoverflow.com/a/27375675/11139119
template <typename Arg, typename... Args>
void doPrint(std::ostream &out, Arg &&arg, Args &&...args) {
  out << std::forward<Arg>(arg);
  using expander = int[];
  (void)expander{0, (void(out << ',' << std::forward<Args>(args)), 0)...};
}

template <size_t... N> void TestIndexSequence(IndexSequence<N...>) {
  doPrint(std::cout, N...);
}

int main() {
  constexpr int arr[] = {1, 2, 3, 4, 4, 5, 5, 5, 6, 7};
  char show0[Power<2, 10, 998244353>::Calculate()];
  char show1[BinarySearch<int>::LowerBound(arr, 4)];
  char show2[BinarySearch<int>::UpperBound(arr, 4)];
  static_assert(Power<2, 10, 998244353>::Calculate() == 1024, "");
  static_assert(BinarySearch<int>::LowerBound(arr, 4) == 3, "");
  static_assert(BinarySearch<int>::UpperBound(arr, 4) == 5, "");
  std::cout << sizeof(show0) << ' ' << sizeof(show1) << ' ' << sizeof(show2)
            << std::endl;
  std::cout << BinarySearch<int>::LowerBound(arr, 5)         // expected 5
            << ' ' << BinarySearch<int>::UpperBound(arr, 5)  // expected 8
            << ' ' << BinarySearch<int>::LowerBound(arr, 7)  // expected 9
            << ' ' << BinarySearch<int>::UpperBound(arr, 7)  // expected 10
            << ' ' << BinarySearch<int>::LowerBound(arr, 8)  // expected 10
            << ' ' << BinarySearch<int>::UpperBound(arr, 8)  // expected 10
            << ' ' << BinarySearch<int>::LowerBound(arr, 1)  // expected 0
            << ' ' << BinarySearch<int>::UpperBound(arr, 1)  // expected 1
            << ' ' << BinarySearch<int>::LowerBound(arr, 0)  // expected 0
            << ' ' << BinarySearch<int>::UpperBound(arr, 0); // expected 0
  std::cout << std::endl;

  Tuple<int, double, std::string> t;
  t.Get<0>() = 15;
  std::cout << t.Get<0>() << std::endl;
  t.Get<1>() = 89.64;
  std::cout << t.Get<1>() << std::endl;
  t.Get<2>() = "string";
  std::cout << t.Get<2>() << std::endl;

  auto tt = MakeTuple(1, "1234", 8.8);
  std::cout << tt.Get<0>() << ' ' << tt.Get<1>() << ' ' << tt.Get<2>()
            << std::endl;
}