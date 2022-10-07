//
// Created by YorkDow Co on 2021/6/7.
//

#include <algorithm>
#include <iterator>

#include "binary_search.h"
#include "examples_util.h"
#include "gtest/gtest.h"
#include "quick_power.h"
#include "reflection.h"
#include "tuple.h"
#include "type_of.h"
#include "currying.h"

using namespace magic;

TEST(TestQuickPower, Main) {
  static_assert(Power<2, 10, 998244353>::Calculate() == 1024, "");
  static_assert(Power<3, 20, 998244353>::Calculate() == 492051342, "");
}

TEST(TestBinarySearch, Main) {
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
  ASSERT_EQ(BinarySearch<int>::LowerBound(arr, 4), std::lower_bound(all(arr), 4) - begin(arr));
  ASSERT_EQ(BinarySearch<int>::UpperBound(arr, 4), std::upper_bound(all(arr), 4) - begin(arr));
  ASSERT_EQ(BinarySearch<int>::LowerBound(arr, 5), std::lower_bound(all(arr), 5) - begin(arr));
  ASSERT_EQ(BinarySearch<int>::UpperBound(arr, 5), std::upper_bound(all(arr), 5) - begin(arr));
  ASSERT_EQ(BinarySearch<int>::LowerBound(arr, 7), std::lower_bound(all(arr), 7) - begin(arr));
  ASSERT_EQ(BinarySearch<int>::UpperBound(arr, 7), std::upper_bound(all(arr), 7) - begin(arr));
  ASSERT_EQ(BinarySearch<int>::LowerBound(arr, 8), std::lower_bound(all(arr), 8) - begin(arr));
  ASSERT_EQ(BinarySearch<int>::UpperBound(arr, 8), std::upper_bound(all(arr), 8) - begin(arr));
  ASSERT_EQ(BinarySearch<int>::LowerBound(arr, 1), std::lower_bound(all(arr), 1) - begin(arr));
  ASSERT_EQ(BinarySearch<int>::UpperBound(arr, 1), std::upper_bound(all(arr), 1) - begin(arr));
  ASSERT_EQ(BinarySearch<int>::LowerBound(arr, 0), std::lower_bound(all(arr), 0) - begin(arr));
  ASSERT_EQ(BinarySearch<int>::UpperBound(arr, 0), std::upper_bound(all(arr), 0) - begin(arr));

  constexpr double float_arr[] = {1.1, 2.2, 3.2, 4.5, 4.56, 5.6, 7};
  static_assert(BinarySearch<double>::LowerBound(float_arr, 5.6) == 5, "");
  ASSERT_EQ(BinarySearch<double>::LowerBound(float_arr, 5.6), std::lower_bound(all(float_arr), 5.6) - begin(float_arr));
}

TEST(TestTuple, Main) {
  Tuple<int, double, std::string> t(1, 1.2, "11.2");
  //  static_assert(t.size() == 3, ""); // can not compile with gcc
  ASSERT_EQ(t.size(), 3);
  ASSERT_EQ(t.Get<0>(), 1);
  ASSERT_EQ(t.Get<1>(), 1.2);
  ASSERT_EQ(t.Get<2>(), "11.2");

  t.Set<0>(15);
  t.Set<1>(89.64);
  t.Set<2>("string");
  ASSERT_EQ(t.Get<0>(), 15);
  ASSERT_EQ(t.Get<1>(), 89.64);
  ASSERT_EQ(t.Get<2>(), "string");

  auto tt = MakeTuple(1, "1234", 8.8);
  //  Down cast to TupleComponent and TupleImpl should be forbidden.
  //  TupleComponent<0, int> &tc = tt;
  //  TupleImpl<IndexSequence<0, 1, 2>, int, const char *, double> &c = tt;
  ASSERT_EQ(tt.Get<0>(), 1);
  //  ASSERT_EQ(tt.Get<1>() == "1234");
  ASSERT_EQ(tt.Get<2>(), 8.8);
}

template <typename Tp>
void func(Tp&& arg) {
  std::cout << TypeInfo<Tp>::info().name() << '\n';
}

TEST(TestNameOf, Main) {
  int vi = 134;
  func(vi);
  func(std::move(vi));
  using base = const volatile int;
  const TypeInfoT* meta = &TypeInfo<base>::info();
  ASSERT_EQ(meta->name(), "const volatile int");
  using wrap1 = base*;
  meta = &TypeInfo<wrap1>::info();
  ASSERT_EQ(meta->name(), "const volatile int*");
  using wrap2 = const wrap1**;
  meta = &TypeInfo<wrap2>::info();
  ASSERT_EQ(meta->name(), "const volatile int*const**");
  using wrap3 = volatile wrap2;
  meta = &TypeInfo<wrap3>::info();
  ASSERT_EQ(meta->name(), "const volatile int*const**volatile");
  ASSERT_EQ(meta->PointerLevels(), 3);

  std::vector<std::map<std::pair<volatile const int*, std::string>, unsigned long long>> testv;
  meta = &TypeInfo<decltype(testv)>::info();
  ASSERT_EQ(meta->name(), "std::vector<std::map<std::pair<const volatile int*,std::string>,unsigned long long>>");
  ASSERT_EQ(meta->id(), TypeEnum::STD_Vector);
  ASSERT_EQ(meta->first()->name(), "std::map<std::pair<const volatile int*,std::string>,unsigned long long>");
  ASSERT_EQ(meta->first()->id(), TypeEnum::STD_Map);

  meta = meta->first();
  ASSERT_EQ(meta->first()->name(), "std::pair<const volatile int*,std::string>");
  ASSERT_EQ(meta->second()->name(), "unsigned long long");

  meta = meta->first();
  ASSERT_EQ(meta->first()->name(), "const volatile int*");
  ASSERT_EQ(meta->second()->name(), "std::string");

  ASSERT_EQ(meta->first()->PointerLevels(), 1);  // is pointer
  meta = meta->first()->first();
  ASSERT_TRUE(meta->IsConst() && meta->IsVolatile());
  ASSERT_EQ(meta->decay()->name(), "int");

  std::vector<std::array<char[2], 10>> testa[5];
  meta = &TypeInfo<decltype(testa)>::info();
  ASSERT_EQ(meta->name(), "std::vector<std::array<char[2],10>>[5]");

  const volatile auto&& testaref = &testa;
  meta = &TypeInfo<decltype(testaref)>::info();
  ASSERT_EQ(meta->name(), "std::vector<std::array<char[2],10>>[5]*const volatile&&");

  std::priority_queue<int, std::deque<int>, std::greater<int>> v;
  meta = &TypeInfo<decltype(v)>::info();
  ASSERT_EQ(meta->name(), "std::priority_queue<int>");

  meta = &TypeInfo<int const[1][2][3]>::info();
  ASSERT_EQ(meta->name(), "const int[1][2][3]");
  ASSERT_EQ(meta->first()->name(), "int[2][3]");
  ASSERT_EQ(meta->first()->first()->name(), "int[3]");
}

class A {
 public:
  A() : f(0.5), pf(new float{14.5}), vi({1, 2, 3}), vd({1.1, 2.2, 3.3}), s("123_f3") {}

  //  private:
  friend class magic::TypeFieldsScheme<A>;
  float f;
  float* pf;
  std::vector<int> vi;
  std::vector<double> vd;
  std::string s;
};

REGISTER_FIELDS(A, FIELD(vi, vi tag), FIELD(vd, vd tag), FIELD(s, s tag), FIELD_NO_TAG(f), FIELD_NO_TAG(pf));

class TypeAReflectHandler {
 public:
  explicit TypeAReflectHandler(const A& val) : a(val) {}

  template <typename T>
  void operator()(T&& val, const char* name, const char* tag) {
    if (std::is_same<decay_t<T>, decltype(a.vi)>{}) {
      EXPECT_EQ(name, "vi");
      EXPECT_EQ(*reinterpret_cast<decltype(a.vi)*>(&val), a.vi);
      EXPECT_EQ(tag, "vi tag");
    } else if (std::is_same<decay_t<T>, decltype(a.vd)>{}) {
      EXPECT_EQ(name, "vd");
      EXPECT_EQ(*reinterpret_cast<decltype(a.vd)*>(&val), a.vd);
      EXPECT_EQ(tag, "vd tag");
    } else if (std::is_same<decay_t<T>, decltype(a.s)>{}) {
      EXPECT_EQ(name, "s");
      EXPECT_EQ(*reinterpret_cast<decltype(a.s)*>(&val), a.s);
      EXPECT_EQ(tag, "s tag");
    } else if (std::is_same<decay_t<T>, decltype(a.f)>{}) {
      EXPECT_EQ(name, "f");
      EXPECT_EQ(*reinterpret_cast<decltype(a.f)*>(&val), a.f);
      EXPECT_EQ(tag, "");
    }
  }

  void operator()(float* val, const char* name, const char* tag) {
    EXPECT_EQ(name, "pf");
    EXPECT_EQ(val, a.pf);
    EXPECT_EQ(tag, "");
  }

 private:
  const A& a;
};

TEST(TestReflection, Main) {
  A a;
  static_assert(TypeFieldsScheme<A>::size == 5, "");
  static_assert(IS_REFLECTABLE(a) == true, "");
  auto tt = MakeTuple(1, 2, 3);
  static_assert(IS_REFLECTABLE(tt) == false, "");

  EXPECT_EQ(NameOf(a), "A");

  EXPECT_EQ(ValueOf<0>(a), a.vi);
  EXPECT_EQ(ValueOf<1>(a), a.vd);
  EXPECT_EQ(ValueOf<2>(a), a.s);

  EXPECT_EQ(NameOf<0>(a), "vi");
  EXPECT_EQ(NameOf<1>(a), "vd");
  EXPECT_EQ(NameOf<2>(a), "s");

  EXPECT_EQ(TypeNameOf<0>(a), TypeInfo<decltype(a.vi)>::info().name());
  EXPECT_EQ(TypeNameOf<1>(a), TypeInfo<decltype(a.vd)>::info().name());
  EXPECT_EQ(TypeNameOf<2>(a), TypeInfo<decltype(a.s)>::info().name());

  ValueOf<0>(a) = std::vector<int>{1};

  ForEachField<TypeAReflectHandler> iterator;
  iterator.Iterate(a, a);

  auto fields = GetAllFields(a);
  for (size_t i = 0; i < fields.size(); i++) {
    auto typeinfo = fields.TypeOf(i);
    switch (typeinfo->id()) {
      case TypeEnum::Float:
        if (typeinfo->PointerLevels() == 0) {
          fields.Set(i, (float)99);
        } else if (typeinfo->PointerLevels() == 1) {
          float* ptr = *reinterpret_cast<float**>(fields.PtrOf(i));
          *ptr = 100;
        }
        break;
      case TypeEnum::STD_Vector:
        switch (typeinfo->first()->id()) {
          case TypeEnum::Int:
            fields.Set(i, std::vector<int>{5, 6, 7, 8});
            break;
          case TypeEnum::Double:
            fields.Set(i, std::vector<double>{5.5, 6.6});
            break;
          default:;
        }
        break;
      case TypeEnum::STD_String:
        fields.Set(i, std::string("yet another string"));
        break;
      default:;
    }
  }

  iterator.Iterate(a, a);
}

TEST(TestCurrying, Main) {
  CURRY(CURRY(std::vector, int), std::allocator<int>) v;
//  std::cout << TypeInfo<decltype(v)>::info().name();
}
