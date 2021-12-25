//
// Created by YorkDow Co on 2021/12/22.
//

#include "test_name_of.h"

using namespace magic;

void TestNameOf() {
  using base = const volatile int;
  const TypeInfoT *meta = &TypeInfo<base>::info();
  assert(meta->name() == "const volatile int");
  using wrap1 = base *;
  meta = &TypeInfo<wrap1>::info();
  assert(meta->name() == "const volatile int*");
  using wrap2 = const wrap1 **;
  meta = &TypeInfo<wrap2>::info();
  assert(meta->name() == "const volatile int*const**");
  using wrap3 = volatile wrap2;
  meta = &TypeInfo<wrap3>::info();
  assert(meta->name() == "const volatile int*const**volatile");
  assert(meta->PointerLevels() == 3);

  std::vector<std::map<std::pair<volatile const int *, std::string>, uint64_t>> testv;
  meta = &TypeInfo<decltype(testv)>::info();
  assert(meta->name() == "std::vector<std::map<std::pair<const volatile int*,std::string>,unsigned long long>>");
  assert(meta->id() == TypeEnum::STD_Vector);
  assert(meta->first()->name() == "std::map<std::pair<const volatile int*,std::string>,unsigned long long>");
  assert(meta->first()->id() == TypeEnum::STD_Map);

  meta = meta->first();
  assert(meta->first()->name() == "std::pair<const volatile int*,std::string>");
  assert(meta->second()->name() == "unsigned long long");

  meta = meta->first();
  assert(meta->first()->name() == "const volatile int*");
  assert(meta->second()->name() == "std::string");

  assert(meta->first()->PointerLevels() == 1);// is pointer
  meta = meta->first()->first();
  assert(meta->IsConst() && meta->IsVolatile());
  assert(meta->decay()->name() == "int");

  std::vector<std::array<char[2], 10>> testa[5];
  meta = &TypeInfo<decltype(testa)>::info();
  assert(meta->name() == "std::vector<std::array<char[2],10>>[5]");

  const volatile auto &&testaref = &testa;
  meta = &TypeInfo<decltype(testaref)>::info();
  assert(meta->name() == "std::vector<std::array<char[2],10>>[5]*const volatile&&");

  std::priority_queue<int, std::deque<int>, std::greater<int>> v;
  meta = &TypeInfo<decltype(v)>::info();
  assert(meta->name() == "std::priority_queue<int>");

  meta = &TypeInfo<int const[1][2][3]>::info();
  assert(meta->name() == "const int[1][2][3]");
  assert(meta->first()->name() == "int[2][3]");
  assert(meta->first()->first()->name() == "int[3]");
}