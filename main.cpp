#include <iostream>
#include <string>
#include <vector>

#include "binary_search.h"
#include "index_sequence.h"
#include "quick_power.h"
#include "reflection.h"
#include "tuple.h"

using namespace magic;

// Override std::ostream so that std::cout could print stl container directly.
template<class T1, class T2>
inline std::ostream &operator<<(std::ostream &os, const std::pair<T1, T2> &v) {
  os << '{' << v.first << ", " << v.second << '}';
  return os;
}

template<template<class ...Args> class ContainerT, class ...Args>
std::ostream &operator<<(std::ostream &os,
                         const ContainerT<Args...> &container) {
  os << '[';
  auto it = container.begin();
  if (it != container.end()) {
    os << *it;
    while (++it != container.end()) {
      os << ' ' << *it;
    }
  }
  os << ']';
  return os;
}

inline std::ostream &operator<<(std::ostream &os, const std::string &str) {
  for (const char &c: str)
    os << c;
  return os;
}

// Help to print IndexSequence
// Source https://stackoverflow.com/a/27375675/11139119
template<class Arg, class ...Args>
void doPrint(std::ostream &out, Arg &&arg, Args &&...args) {
  out << std::forward<Arg>(arg);
  using expander = int[];
  (void) expander{0, (void(out << ',' << std::forward<Args>(args)), 0)...};
}

template<size_t... N>
void TestIndexSequence(IndexSequence<N...>) {
  doPrint(std::cout, N...);
}

class A {
 public:
  A() : a(15), b(21), vi({1, 2, 3}), vd({1.1, 2.2, 3.3}), s("123_f3") {}

 private:
  friend class TypeFieldsScheme<A>;
  int a;
  int b;
  std::vector<int> vi;
  std::vector<double> vd;
  std::string s;
};

RegisterFields(A, Field(vi, std::vector<int>), Field(vd, std::vector<double>),
               Field(s, std::string));

class ReflectHandler {
 public:
  template<typename T>
  void operator()(T &&var, const char *name, const char *type) {
    std::cout << std::fixed << type << ' ' << name << ' ' << var << ' '
              << std::endl;
  }
};

int main() {
  constexpr int arr[] = {1, 2, 3, 4, 4, 5, 5, 5, 6, 7};
  char show0[Power<2, 10, 998244353>::Calculate()];
  char show1[BinarySearch<int>::LowerBound(arr, 4)];
  char show2[BinarySearch<int>::UpperBound(arr, 4)];
  static_assert(Power<3, 20, 998244353>::Calculate() == 492051342, "");
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
//  Down cast to TupleComponent and TupleImpl should be forbidden.
//  TupleComponent<0, int> &tc = tt;
//  TupleImpl<IndexSequence<0, 1, 2>, int, const char *, double> &c = tt;
  std::cout << tt.Get<0>() << ' ' << tt.Get<1>() << ' ' << tt.Get<2>()
            << std::endl;

  ForEachField<ReflectHandler> iterator;
  iterator(A{});
}