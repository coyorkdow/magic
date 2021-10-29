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

enum NameEnum {
  Unknown,
  UnsignedShort,
  UnsignedInt,
  UnsignedLongLong,
  SignedChar,
  Short,
  Int,
  LongLong,
  UnsignedChar,
  Char,
  Wchar_T,
  Long,
  UnsignedLong,
  Void,
  Char16_T,
  Char32_T,
  Float,
  Double,
  LongDouble,

  Array,

  STD_String,
  STD_Vector,
  STD_Array,
  STD_Pair,
  STD_Map
};

class Any {
 public:
  Any() = default;
  Any(NameEnum id,
      std::string (*name)(),
      int ptr_cnt = 0,
      int ref_type = 0,
      int cv_qualifier = 0,
      size_t size = 0,
      const Any *first_stored = nullptr,
      const Any *second_stored = nullptr) :
      name(name),
      id_(id),
      ptr_cnt_(ptr_cnt),
      ref_type_(ref_type),
      cv_qualifier_(cv_qualifier),
      size_(size),
      first_stored(first_stored),
      second_stored(second_stored) {}

  inline bool IsLvalueReference() const { return ref_type_ == 1; }
  inline bool IsRValueReference() const { return ref_type_ == 2; }

  inline bool IsConst() const { return cv_qualifier_ & 1; }
  inline bool IsVolatile() const { return cv_qualifier_ & 2; }

  std::string (*const name)() = []() -> std::string { return "unknown"; };

  inline NameEnum id() const { return id_; }
  inline int ptr_cnt() const { return ptr_cnt_; }
  inline int ref_type() const { return ref_type_; }
  inline int cv_qualifier() const { return cv_qualifier_; }
  inline size_t size() const { return size_; }

 private:
  const NameEnum id_ = Unknown;
  const int ptr_cnt_ = 0;
  const int ref_type_ = 0; // 0: not ref, 1: lvalue ref, 2: right ref
  const int cv_qualifier_ = 0;
  const size_t size_ = 0; // for Array and STD_Array

 public:
  const Any *const first_stored = nullptr; // for container
  const Any *const second_stored = nullptr; // for container
};

template<class Tp>
struct TypeInfo { static const Any info; };

template<class Tp> const Any TypeInfo<Tp>::info;

#define REGISTER_POD_TYPE(Tp, Enum) \
  template <> struct TypeInfo<Tp> {     \
    static const Any info;             \
  };                                \
  const Any TypeInfo<Tp>::info(Enum, []() -> std::string { return #Tp; });

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

// ========================== TypeInfo with cv qualifier begin ==========================

template<class Tp>
struct TypeInfo<const Tp> { static const Any info; };

template<class Tp>
const Any TypeInfo<const Tp>::info(
    TypeInfo<Tp>::info.id(),
    NAME_LAMBDA_WRAP("const " + TypeInfo<Tp>::info.name()),
    TypeInfo<Tp>::info.ptr_cnt(),
    TypeInfo<Tp>::info.ref_type(),
    1,
    TypeInfo<Tp>::info.size(),
    &TypeInfo<Tp>::info);

template<class Tp>
struct TypeInfo<volatile Tp> { static const Any info; };

template<class Tp>
const Any TypeInfo<volatile Tp>::info(
    TypeInfo<Tp>::info.id(),
    NAME_LAMBDA_WRAP("volatile " + TypeInfo<Tp>::info.name()),
    TypeInfo<Tp>::info.ptr_cnt(),
    TypeInfo<Tp>::info.ref_type(),
    2,
    TypeInfo<Tp>::info.size(),
    &TypeInfo<Tp>::info);

template<class Tp>
struct TypeInfo<const volatile Tp> { static const Any info; };

template<class Tp>
const Any TypeInfo<const volatile Tp>::info(
    TypeInfo<Tp>::info.id(),
    NAME_LAMBDA_WRAP("const volatile " + TypeInfo<Tp>::info.name()),
    TypeInfo<Tp>::info.ptr_cnt(),
    TypeInfo<Tp>::info.ref_type(),
    3,
    TypeInfo<Tp>::info.size(),
    &TypeInfo<Tp>::info);

// ========================== TypeInfo with cv qualifier end ==========================


// ======================= TypeInfo with ptr and reference begin =======================

template<class Tp>
struct TypeInfo<Tp *> { static const Any info; };

template<class Tp>
const Any TypeInfo<Tp *>::info(
    TypeInfo<Tp>::info.id(),
    NAME_LAMBDA_WRAP(TypeInfo<Tp>::info.name() + "*"),
    TypeInfo<Tp>::info.ptr_cnt() + 1,
    0,
    TypeInfo<Tp>::info.cv_qualifier(),
    TypeInfo<Tp>::info.size(),
    &TypeInfo<Tp>::info);

template<class Tp>
struct TypeInfo<Tp &> { static const Any info; };

template<class Tp>
const Any TypeInfo<Tp &>::info(
    TypeInfo<Tp>::info.id(),
    NAME_LAMBDA_WRAP(TypeInfo<Tp>::info.name() + "&"),
    TypeInfo<Tp>::info.ptr_cnt(),
    1,
    TypeInfo<Tp>::info.cv_qualifier(),
    TypeInfo<Tp>::info.size(),
    &TypeInfo<Tp>::info);

template<class Tp>
struct TypeInfo<Tp &&> { static const Any info; };

template<class Tp>
const Any TypeInfo<Tp &&>::info(
    TypeInfo<Tp>::info.id(),
    NAME_LAMBDA_WRAP(TypeInfo<Tp>::info.name() + "&"),
    TypeInfo<Tp>::info.ptr_cnt(),
    2,
    TypeInfo<Tp>::info.cv_qualifier(),
    TypeInfo<Tp>::info.size(),
    &TypeInfo<Tp>::info);


// ======================= TypeInfo with ptr and reference end =======================


// =============================== Array type begin ===============================

template<class Tp, size_t Size>
struct TypeInfo<Tp[Size]> { static const Any info; };

template<class Tp, size_t Size>
const Any TypeInfo<Tp[Size]>::info(
    Array,
    NAME_LAMBDA_WRAP(TypeInfo<Tp>::info.name() + "[" + std::to_string(Size) + "]"),
    TypeInfo<Tp>::info.ptr_cnt(),
    TypeInfo<Tp>::info.ref_type(),
    0,
    Size,
    &TypeInfo<Tp>::info);

template<class Tp, size_t Size>
struct TypeInfo<const Tp[Size]> { static const Any info; };

template<class Tp, size_t Size>
const Any TypeInfo<const Tp[Size]>::info(
    Array,
    NAME_LAMBDA_WRAP("const " + TypeInfo<Tp>::info.name() + "[" + std::to_string(Size) + "]"),
    TypeInfo<Tp>::info.ptr_cnt(),
    TypeInfo<Tp>::info.ref_type(),
    1,
    Size,
    &TypeInfo<Tp>::info);

template<class Tp, size_t Size>
struct TypeInfo<volatile Tp[Size]> { static const Any info; };

template<class Tp, size_t Size>
const Any TypeInfo<volatile Tp[Size]>::info(
    Array,
    NAME_LAMBDA_WRAP("volatile " + TypeInfo<Tp>::info.name() + "[" + std::to_string(Size) + "]"),
    TypeInfo<Tp>::info.ptr_cnt(),
    TypeInfo<Tp>::info.ref_type(),
    2,
    Size,
    &TypeInfo<Tp>::info);

template<class Tp, size_t Size>
struct TypeInfo<const volatile Tp[Size]> { static const Any info; };

template<class Tp, size_t Size>
const Any TypeInfo<const volatile Tp[Size]>::info(
    Array,
    NAME_LAMBDA_WRAP("const volatile " + TypeInfo<Tp>::info.name() + "[" + std::to_string(Size) + "]"),
    TypeInfo<Tp>::info.ptr_cnt(),
    TypeInfo<Tp>::info.ref_type(),
    3,
    Size,
    &TypeInfo<Tp>::info);

// =============================== Array type end ===============================

template<> struct TypeInfo<std::string> { static const Any info; };

const Any TypeInfo<std::string>::info(STD_String, NAME_LAMBDA_WRAP("std::string"));

template<class Tp>
struct TypeInfo<std::vector<Tp>> { static const Any info; };

template<class Tp>
const Any TypeInfo<std::vector<Tp>>::info(
    STD_Vector,
    NAME_LAMBDA_WRAP("std::vector<" + TypeInfo<Tp>::info.name() + ">"),
    0,
    0,
    0,
    0,
    &TypeInfo<Tp>::info);

template<class Tp, size_t Size>
struct TypeInfo<std::array<Tp, Size>> { static const Any info; };

template<class Tp, size_t Size>
const Any TypeInfo<std::array<Tp, Size>>::info(
    STD_Array,
    NAME_LAMBDA_WRAP("std::array<" + TypeInfo<Tp>::info.name() + "," + std::to_string(Size) + ">"),
    0,
    0,
    0,
    Size,
    &TypeInfo<Tp>::info);

template<class T1, class T2>
struct TypeInfo<std::pair<T1, T2>> { static const Any info; };

template<class T1, class T2>
const Any TypeInfo<std::pair<T1, T2>>::info(
    STD_Pair,
    NAME_LAMBDA_WRAP("std::pair<" + TypeInfo<T1>::info.name() + "," + TypeInfo<T2>::info.name() + ">"),
    0,
    0,
    0,
    0,
    &TypeInfo<T1>::info,
    &TypeInfo<T2>::info);

template<class T1, class T2>
struct TypeInfo<std::map<T1, T2>> { static const Any info; };

template<class T1, class T2>
const Any TypeInfo<std::map<T1, T2>>::info(
    STD_Map,
    NAME_LAMBDA_WRAP("std::map<" + TypeInfo<T1>::info.name() + "," + TypeInfo<T2>::info.name() + ">"),
    0,
    0,
    0,
    0,
    &TypeInfo<T1>::info,
    &TypeInfo<T2>::info);


}  // namespace magic

#endif  // MAGIC__NAME_OF_H_
