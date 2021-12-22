//
// Created by YorkDow Co on 2021/12/22.
//

#include "test_tuple.h"

#include <string>

using namespace magic;

void TestTuple() {
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
}
