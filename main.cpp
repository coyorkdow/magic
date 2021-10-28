#include <algorithm>
#include <cassert>
#include <iostream>
#include <iomanip>
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
  static const size_t seq[] = {N...};
  for (auto each: seq) std::cout << each << ' ';
  std::cout << std::endl;
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

RegisterFields(A, Field(vi, vi tag), Field(vd, vd tag), Field(s, s tag));

class ReflectHandler {
 public:
  template<typename T>
  void operator()(T &&var, const char *name, const char *tag) {
    std::cout << "tag: " << std::setw(8) << tag;
    std::cout << "│name: " << std::setw(8) << name;
    std::cout << "│value: " << var << '\n';
  }
};

int main() {
  constexpr int arr[] = {1, 2, 3, 4, 4, 5, 5, 5, 6, 7};
  char show0[Power<2, 10, 998244353>::Calculate()];
  char show1[BinarySearch<int>::LowerBound(arr, 4)];
  char show2[BinarySearch<int>::UpperBound(arr, 4)];
  static_assert(sizeof(show0) == 1024, "");
  static_assert(sizeof(show1) == 3, "");
  static_assert(sizeof(show2) == 5, "");
  static_assert(Power<3, 20, 998244353>::Calculate() == 492051342, "");

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

  TestIndexSequence(MakeIndexSequence<10>());

  Tuple<int, double, std::string> t(1, 1.2, "11.2");
//  static_assert(t.size() == 3, ""); // can not compile with gcc
  assert(t.size() == 3);
  assert(t.Get<0>() == 1);
  assert(t.Get<1>() == 1.2);
  assert(t.Get<2>() == "11.2");

  t.Set<0>(15);
  t.Set<1>(89.64);
  t.Set<2>("string");
  assert(t.Get<0>() == 15);
  assert(t.Get<1>() == 89.64);
  assert(t.Get<2>() == "string");

  auto tt = MakeTuple(1, "1234", 8.8);
//  Down cast to TupleComponent and TupleImpl should be forbidden.
//  TupleComponent<0, int> &tc = tt;
//  TupleImpl<IndexSequence<0, 1, 2>, int, const char *, double> &c = tt;
  assert(tt.Get<0>() == 1);
//  assert(tt.Get<1>() == "1234");
  assert(tt.Get<2>() == 8.8);

  A a;
  static_assert(TypeFieldsScheme<A>::size == 3, "");
  static_assert(IsReflectable(a) == true, "");
  static_assert(IsReflectable(tt) == false, "");
  std::cout << NameOf(a) << std::endl;
  std::cout << ValueOf<0>(a) << ' ' << ValueOf<1>(a) << ' ' << ValueOf<2>(a) << std::endl;
  std::cout << NameOf<0>(a) << ' ' << NameOf<1>(a) << ' ' << NameOf<2>(a) << std::endl;
  std::cout << TypeNameOf<0>(a) << ' ' << TypeNameOf<1>(a) << ' ' << TypeNameOf<2>(a) << std::endl;
  ValueOf<0>(a) = std::vector<int>{1};

  ForEachField<ReflectHandler> iterator;
  iterator.Iterate(a);

  auto fields = GetAllFields(a);
  for (size_t i = 0; i < fields.size(); i++) {
    if (fields.TagOf(i) == "vi tag") {
      fields.Set(i, std::vector<int>{5, 6, 7, 8});
    } else if (fields.TagOf(i) == "vd tag") {
      fields.Set(i, std::vector<double>{5.5, 6.6});
    } else if (fields.TagOf(i) == "s tag") {
      fields.Set(i, std::string("yet another string"));
    }
  }

  iterator.Iterate(a);

  std::vector<std::map<std::pair<int,std::string>, uint64_t>> testv;
  const Any *meta = &Type<decltype(testv)>::m;
  std::cout << std::endl << "type of testv is " << meta->name();
  std::cout << std::endl << "type of stored in testv is " << meta->first_stored->name();
  meta = meta->first_stored;
  std::cout << std::endl << "type of key of map is " << meta->first_stored->name();
  std::cout << std::endl << "type of val of map is " << meta->second_stored->name();
  meta = meta->first_stored;
  std::cout << std::endl << "type of first of pair is " << meta->first_stored->name();
  std::cout << std::endl << "type of second of pair is " << meta->second_stored->name();


  std::array<std::vector<const int*>, 10> testa[5];
  std::cout << std::endl << "type of testa is " << Type<decltype(testa)>::m.name();

  auto &&testaref = &testa;
  std::cout << std::endl << "type of testaref is " << Type<decltype(testaref)>::m.name();
}