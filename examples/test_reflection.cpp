//
// Created by YorkDow Co on 2021/12/22.
//

#include "test_reflection.h"

#include <iomanip>
#include <iostream>

#include "examples_util.h"

using namespace magic;

class ReflectHandler {
 public:
  template<typename T>
  void operator()(T &&var, const char *name, const char *tag) {
    std::cout << "tag: " << std::setw(8) << tag;
    std::cout << "│name: " << std::setw(8) << name;
    std::cout << "│value: " << var << '\n';
  }
  void operator()(float *var, const char *name, const char *tag) {
    std::cout << "tag: " << std::setw(8) << tag;
    std::cout << "│name: " << std::setw(8) << name;
    std::cout << "│value: " << *var << '\n';
  }
};

void TestReflection() {
  A a;
  static_assert(TypeFieldsScheme<A>::size == 5, "");
  static_assert(IS_REFLECTABLE(a) == true, "");
  auto tt = MakeTuple(1, 2, 3);
  static_assert(IS_REFLECTABLE(tt) == false, "");
  std::cout << NameOf(a) << std::endl;
  std::cout << ValueOf<0>(a) << ' ' << ValueOf<1>(a) << ' ' << ValueOf<2>(a) << std::endl;
  std::cout << NameOf<0>(a) << ' ' << NameOf<1>(a) << ' ' << NameOf<2>(a) << std::endl;
  std::cout << TypeNameOf<0>(a) << ' ' << TypeNameOf<1>(a) << ' ' << TypeNameOf<2>(a) << std::endl;
  ValueOf<0>(a) = std::vector<int>{1};

  ForEachField<ReflectHandler> iterator;
  iterator.Iterate(a);
  std::cout << std::endl;

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

  iterator.Iterate(a);
}
