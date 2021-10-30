# magic
Fantasic magic of modern c++

This reposity is aimed to show the power of modern c++ features. All codes are compliant the standard of C++11. I would say sorry if you are a favor of C++14/C++17/C++20. LOL.

Usage can be found in `main.cpp`.

`quick_power.h`

Calculate the exponent in compile time with a time complexity of `O(log n)`,

**Example**
```C++
static_assert(Power<2, 10, 998244353>::Calculate() == 1024, "");
static_assert(Power<3, 20, 998244353>::Calculate() == 492051342, "");
```

`binary_search.h`

Binary searching in compile time.

**Example**
```C++
constexpr int arr[] = {1, 2, 3, 4, 4, 5, 5, 5, 6, 7};

static_assert(BinarySearch<int>::LowerBound(arr, 4) == 3, "");
static_assert(BinarySearch<int>::UpperBound(arr, 4) == 5, "");
```

`index_sequence.h`

Generate a compile time integer sequence, as well as `std::index_sequence` in C++14.

**Example**
```C++
template<size_t... N>
void TestIndexSequence(IndexSequence<N...>) {
  static const size_t seq[] = {N...};
  for (auto each : seq) std::cout << each << ' ';
  std::cout << std::endl;
}

// print 0 1 2 3 4 5 6 7 8 9 
TestIndexSequence(MakeIndexSequence<10>());
```

`tuple.h`

A simple implementation of tuple, rely on `index_sequence.h`.

**Example**
```C++
Tuple<int, double, std::string> t(1, 1.2, "11.2");
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
```

`name_of.h`

Get type info during runtime.

**Example**
```C++
std::vector<std::map<std::pair<volatile const int *, std::string>, uint64_t>> testv;
meta = &TypeInfo<decltype(testv)>::info;
assert(meta->name() == "std::vector<std::map<std::pair<const volatile int*,std::string>,unsigned long long>>");
assert(meta->id() == NameEnum::STD_Vector);
assert(meta->first()->name() == "std::map<std::pair<const volatile int*,std::string>,unsigned long long>");
assert(meta->first()->id() == NameEnum::STD_Map);

meta = meta->first();
assert(meta->first()->name() == "std::pair<const volatile int*,std::string>");
assert(meta->second()->name() == "unsigned long long");

meta = meta->first();
assert(meta->first()->name() == "const volatile int*");
assert(meta->second()->name() == "std::string");

assert(meta->first()->PointerLevels() == 1); // is pointer
meta = meta->first()->first();
assert(meta->IsConst() && meta->IsVolatile());
assert(meta->decay()->name() == "int");
```

`reflection.h`

A simple implementation of reflection, rely on `tuple.h` and `name_of.h`.

```C++
auto fields = GetAllFields(a);
for (size_t i = 0; i < fields.size(); i++) {
  auto typeinfo = fields.TypeOf(i);
  switch (typeinfo->id()) {
    case NameEnum::Float:
      if (typeinfo->PointerLevels() == 0) {
        fields.Set(i, (float) 99);
      } else if (typeinfo->PointerLevels() == 1) {
        float *ptr = *reinterpret_cast<float **>(fields.PtrOf(i));
        *ptr = 100;
      }
      break;
    case NameEnum::STD_Vector:
      switch (typeinfo->first()->id()) {
        case NameEnum::Int:
          fields.Set(i, std::vector<int>{5, 6, 7, 8});
          break;
        case NameEnum::Double:
          fields.Set(i, std::vector<double>{5.5, 6.6});
          break;
        default:;
      }
      break;
    case NameEnum::STD_String:
      fields.Set(i, std::string("yet another string"));
      break;
    default:;
  }
}
```
