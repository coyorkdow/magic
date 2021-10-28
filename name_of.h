//
// Created by YorkDow Co on 2021/10/28.
//

#ifndef MAGIC__NAME_OF_H_
#define MAGIC__NAME_OF_H_

#include <utility>
#include <vector>
#include <array>
#include <queue>
#include <deque>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <string>

namespace magic {

using NameEnum = const unsigned int;

NameEnum Unknown = 0;

NameEnum UnsignedShort = 1;
NameEnum UnsignedInt = 2;
NameEnum UnsignedLongLong = 3;
NameEnum SignedChar = 4;
NameEnum Short = 5;
NameEnum Int = 6;
NameEnum LongLong = 7;
NameEnum UnsignedChar = 8;
NameEnum Char = 9;
NameEnum Wchar_T = 10;
NameEnum Long = 11;
NameEnum UnsignedLong = 12;
NameEnum Void = 13;
NameEnum Char16_T = 14;
NameEnum Char32_T = 15;
NameEnum Float = 16;
NameEnum Double = 17;
NameEnum LongDouble = 18;

NameEnum Array = 30;

NameEnum String = 50;
NameEnum Vector = 51;
NameEnum STLArray = 52;
NameEnum Deque = 53;
NameEnum Pair = 60;
NameEnum Map = 61;

class Any {
 public:
  Any() = default;
  Any(NameEnum id_,
      std::string (*name_)(),
      int ptr_cnt_ = 0,
      int ref_type_ = 0,
      int cv_qualifier_ = 0,
      size_t size_ = 0,
      const Any *stored_ = nullptr,
      const Any *stored2_ = nullptr) :
      name(name_),
      id(id_),
      ptr_cnt(ptr_cnt_),
      ref_type(ref_type_),
      cv_qualifier(cv_qualifier_),
      size(size_),
      first_stored(stored_),
      second_stored(stored2_) {}

  std::string (*const name)() = []() -> std::string { return "unknown"; };
  const NameEnum id = 0;
  const int ptr_cnt = 0;
  const int ref_type = 0; // 0: not ref, 1: lvalue ref, 2: right ref
  const int cv_qualifier = 0;
  const size_t size = 0; // for Array and STLArray
  const Any *const first_stored = nullptr; // for container
  const Any *const second_stored = nullptr; // for container
};

template<class Tp>
struct Type { static const Any m; };

#define REGISTER_POD_TYPE(Tp, Enum) \
  template <> struct Type<Tp> {     \
    static const Any m;             \
  };                                \
  const Any Type<Tp>::m(Enum, []() -> std::string { return #Tp; });

REGISTER_POD_TYPE(unsigned short, UnsignedShort)
REGISTER_POD_TYPE(unsigned int, UnsignedInt)
REGISTER_POD_TYPE(unsigned long long, UnsignedLongLong)
REGISTER_POD_TYPE(signed char, SignedChar)
REGISTER_POD_TYPE(short, Short)
REGISTER_POD_TYPE(int, Int)
REGISTER_POD_TYPE(long long, LongLong)
REGISTER_POD_TYPE(unsigned char, UnsignedChar)
REGISTER_POD_TYPE(char, Char)
REGISTER_POD_TYPE(wchar_t, Wchar_T)
REGISTER_POD_TYPE(long, Long)
REGISTER_POD_TYPE(unsigned long, UnsignedLong)
REGISTER_POD_TYPE(void, Void)
REGISTER_POD_TYPE(char16_t, Char16_T)
REGISTER_POD_TYPE(char32_t, Char32_T)
REGISTER_POD_TYPE(float, Float)
REGISTER_POD_TYPE(double, Double)
REGISTER_POD_TYPE(long double, LongDouble)

#define NAME_LAMBDA_WRAP(exp) []() -> std::string { return exp; }

// ========================== Type with cv qualifier begin ==========================

template<class Tp>
struct Type<const Tp> { static const Any m; };

template<class Tp>
const Any Type<const Tp>::m(
    Type<Tp>::m.id,
    NAME_LAMBDA_WRAP("const " + Type<Tp>::m.name()),
    Type<Tp>::m.ptr_cnt,
    Type<Tp>::m.ref_type,
    1,
    Type<Tp>::m.size,
    &Type<Tp>::m);

template<class Tp>
struct Type<volatile Tp> { static const Any m; };

template<class Tp>
const Any Type<volatile Tp>::m(
    Type<Tp>::m.id,
    NAME_LAMBDA_WRAP("volatile " + Type<Tp>::m.name()),
    Type<Tp>::m.ptr_cnt,
    Type<Tp>::m.ref_type,
    2,
    Type<Tp>::m.size,
    &Type<Tp>::m);

template<class Tp>
struct Type<const volatile Tp> { static const Any m; };

template<class Tp>
const Any Type<const volatile Tp>::m(
    Type<Tp>::m.id,
    NAME_LAMBDA_WRAP("const volatile " + Type<Tp>::m.name()),
    Type<Tp>::m.ptr_cnt,
    Type<Tp>::m.ref_type,
    3,
    Type<Tp>::m.size,
    &Type<Tp>::m);

// ========================== Type with cv qualifier end ==========================


// ======================= Type with ptr and reference begin =======================

template<class Tp>
struct Type<Tp *> { static const Any m; };

template<class Tp>
const Any Type<Tp *>::m(
    Type<Tp>::m.id,
    NAME_LAMBDA_WRAP(Type<Tp>::m.name() + "*"),
    Type<Tp>::m.ptr_cnt + 1,
    0,
    Type<Tp>::m.cv_qualifier,
    Type<Tp>::m.size,
    &Type<Tp>::m);

template<class Tp>
struct Type<Tp &> { static const Any m; };

template<class Tp>
const Any Type<Tp &>::m(
    Type<Tp>::m.id,
    NAME_LAMBDA_WRAP(Type<Tp>::m.name() + "&"),
    Type<Tp>::m.ptr_cnt,
    1,
    Type<Tp>::m.cv_qualifier,
    Type<Tp>::m.size,
    &Type<Tp>::m);

template<class Tp>
struct Type<Tp &&> { static const Any m; };

template<class Tp>
const Any Type<Tp &&>::m(
    Type<Tp>::m.id,
    NAME_LAMBDA_WRAP(Type<Tp>::m.name() + "&"),
    Type<Tp>::m.ptr_cnt,
    2,
    Type<Tp>::m.cv_qualifier,
    Type<Tp>::m.size,
    &Type<Tp>::m);


// ======================= Type with ptr and reference end =======================


// =============================== Array type begin ===============================

template<class Tp, size_t Size>
struct Type<Tp[Size]> { static const Any m; };

template<class Tp, size_t Size>
const Any Type<Tp[Size]>::m(
    Array,
    NAME_LAMBDA_WRAP(Type<Tp>::m.name() + "[" + std::to_string(Size) + "]"),
    Type<Tp>::m.ptr_cnt,
    Type<Tp>::m.ref_type,
    0,
    Size,
    &Type<Tp>::m);

template<class Tp, size_t Size>
struct Type<const Tp[Size]> { static const Any m; };

template<class Tp, size_t Size>
const Any Type<const Tp[Size]>::m(
    Array,
    NAME_LAMBDA_WRAP("const " + Type<Tp>::m.name() + "[" + std::to_string(Size) + "]"),
    Type<Tp>::m.ptr_cnt,
    Type<Tp>::m.ref_type,
    1,
    Size,
    &Type<Tp>::m);

template<class Tp, size_t Size>
struct Type<volatile Tp[Size]> { static const Any m; };

template<class Tp, size_t Size>
const Any Type<volatile Tp[Size]>::m(
    Array,
    NAME_LAMBDA_WRAP("volatile " + Type<Tp>::m.name() + "[" + std::to_string(Size) + "]"),
    Type<Tp>::m.ptr_cnt,
    Type<Tp>::m.ref_type,
    2,
    Size,
    &Type<Tp>::m);

template<class Tp, size_t Size>
struct Type<const volatile Tp[Size]> { static const Any m; };

template<class Tp, size_t Size>
const Any Type<const volatile Tp[Size]>::m(
    Array,
    NAME_LAMBDA_WRAP("const volatile " + Type<Tp>::m.name() + "[" + std::to_string(Size) + "]"),
    Type<Tp>::m.ptr_cnt,
    Type<Tp>::m.ref_type,
    3,
    Size,
    &Type<Tp>::m);

// =============================== Array type end ===============================

template<> struct Type<std::string> { static const Any m; };

const Any Type<std::string>::m(String, NAME_LAMBDA_WRAP("std::string"));

template<class Tp>
struct Type<std::vector<Tp>> { static const Any m; };

template<class Tp>
const Any Type<std::vector<Tp>>::m(
    Vector,
    NAME_LAMBDA_WRAP("std::vector<" + Type<Tp>::m.name() + ">"),
    0,
    0,
    0,
    0,
    &Type<Tp>::m);

template<class Tp, size_t Size>
struct Type<std::array<Tp, Size>> { static const Any m; };

template<class Tp, size_t Size>
const Any Type<std::array<Tp, Size>>::m(
    STLArray,
    NAME_LAMBDA_WRAP("std::array<" + Type<Tp>::m.name() + "," + std::to_string(Size) + ">"),
    0,
    0,
    0,
    Size,
    &Type<Tp>::m);

template<class T1, class T2>
struct Type<std::pair<T1, T2>> { static const Any m; };

template<class T1, class T2>
const Any Type<std::pair<T1, T2>>::m(
    Pair,
    NAME_LAMBDA_WRAP("std::pair<" + Type<T1>::m.name() + "," + Type<T2>::m.name() + ">"),
    0,
    0,
    0,
    0,
    &Type<T1>::m,
    &Type<T2>::m);

template<class T1, class T2>
struct Type<std::map<T1, T2>> { static const Any m; };

template<class T1, class T2>
const Any Type<std::map<T1, T2>>::m(
    Map,
    NAME_LAMBDA_WRAP("std::map<" + Type<T1>::m.name() + "," + Type<T2>::m.name() + ">"),
    0,
    0,
    0,
    0,
    &Type<T1>::m,
    &Type<T2>::m);


}  // namespace magic

#endif  // MAGIC__NAME_OF_H_
