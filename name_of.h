//
// Created by YorkDow Co on 2021/10/28.
//

#ifndef MAGIC__NAME_OF_H_
#define MAGIC__NAME_OF_H_

#include <array>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

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
  Bool,
  Char16_T,
  Char32_T,
  Float,
  Double,
  LongDouble,
  STD_NullptrT,

  Array,

  STD_String,
  STD_Vector,
  STD_Array,
  STD_List,
  STD_Stack,
  STD_Queue,
  STD_Deque,
  STD_PriorityQueue,
  STD_Pair,
  STD_Map,
  STD_Set,
  STD_UnorderedMap,
  STD_UnorderedSet,
  STD_Multimap,
  STD_MultiSet,
  STD_UnorderedMultimap,
  STD_UnorderedMultiset
};

class Any {
  template<class Tp>
  friend struct TypeInfo;

 public:
  virtual NameEnum id() const { return NameEnum::Unknown; }

  virtual const std::string &name() const {
    static std::string name = "unknown";
    return name;
  }

  virtual const Any *decay() const { return this; }

  virtual int PointerLevels() const { return 0; }

  virtual bool IsLvalueReference() const { return false; }

  virtual bool IsRvalueReference() const { return false; }

  virtual bool IsConst() const { return false; }

  virtual bool IsVolatile() const { return false; }

  virtual size_t size() const { return 0; }// for Array and STD_Array

  virtual const Any *first() const { return nullptr; }// for container

  virtual const Any *second() const { return nullptr; }// for container

  virtual ~Any() = default;

  Any(const Any &) = delete;

  Any(Any &&) = delete;

 protected:
  Any() = default;

  virtual const std::string &ArraySizeName() const {
    static std::string name;
    return name;
  }
};

#define RETURN(type, v) \
  static type ret = v;  \
  return ret;

#define MAKE_ID(v) \
  NameEnum id() const override { RETURN(NameEnum, v) }

#define MAKE_NAME(exp) \
  const std::string &name() const override { RETURN(std::string, exp) }

#define MAKE_POINTER_LEVELS(v) \
  int PointerLevels() const override { RETURN(int, v) }

#define MAKE_IS_LVALUE_REFERENCE(v) \
  bool IsLvalueReference() const override { RETURN(bool, v) }

#define MAKE_IS_RVALUE_REFERENCE(v) \
  bool IsRvalueReference() const override { RETURN(bool, v) }

#define MAKE_IS_CONST(v) \
  bool IsConst() const override { RETURN(bool, v) }

#define MAKE_IS_VOLATILE(v) \
  bool IsVolatile() const override { RETURN(bool, v) }

#define MAKE_SIZE(v) \
  size_t size() const override { RETURN(size_t, v) }

#define MAKE_DECAY(v) \
  const Any *decay() const override { RETURN(const Any *, v) }

#define MAKE_FIRST(ptr) \
  const Any *first() const override { RETURN(const Any *, ptr) }

#define MAKE_SECOND(ptr) \
  const Any *second() const override { RETURN(const Any *, ptr) }

#define MAKE_ARRAY_SIZE_NAME(exp) \
  const std::string &ArraySizeName() const override { RETURN(std::string, exp) }

template<class Tp>
struct TypeInfo { static const Any info; };

template<class Tp>
const Any TypeInfo<Tp>::info;

#define REGISTER_FUNDAMENTAL_TYPE(Tp, Enum) \
  template<>                                \
  struct TypeInfo<Tp> {                     \
   private:                                 \
    class AnyImpl : public Any {            \
     public:                                \
      MAKE_ID(Enum)                         \
      MAKE_NAME(#Tp)                        \
    };                                      \
                                            \
   public:                                  \
    static const AnyImpl info;              \
  };                                        \
  const TypeInfo<Tp>::AnyImpl TypeInfo<Tp>::info;

#define REGISTER_CONTAINER(Tp, Enum)                       \
  template<class T1, class... T2>                          \
  struct TypeInfo<Tp<T1, T2...>> {                         \
   private:                                                \
    class AnyImpl : public Any {                           \
     public:                                               \
      MAKE_ID(Enum)                                        \
      MAKE_NAME(#Tp "<" + TypeInfo<T1>::info.name() + ">") \
      MAKE_FIRST(&TypeInfo<T1>::info)                      \
    };                                                     \
                                                           \
   public:                                                 \
    static const AnyImpl info;                             \
  };                                                       \
  template<class T1, class... T2>                          \
  const typename TypeInfo<Tp<T1, T2...>>::AnyImpl TypeInfo<Tp<T1, T2...>>::info;

#define REGISTER_PAIR_CONTAINER(Tp, Enum)                                                    \
  template<class T1, class T2, class... T3>                                                  \
  struct TypeInfo<Tp<T1, T2, T3...>> {                                                       \
   private:                                                                                  \
    class AnyImpl : public Any {                                                             \
     public:                                                                                 \
      MAKE_ID(Enum)                                                                          \
      MAKE_NAME(#Tp "<" + TypeInfo<T1>::info.name() + "," + TypeInfo<T2>::info.name() + ">") \
      MAKE_FIRST(&TypeInfo<T1>::info)                                                        \
      MAKE_SECOND(&TypeInfo<T2>::info)                                                       \
    };                                                                                       \
                                                                                             \
   public:                                                                                   \
    static const AnyImpl info;                                                               \
  };                                                                                         \
  template<class T1, class T2, class... T3>                                                  \
  const typename TypeInfo<Tp<T1, T2, T3...>>::AnyImpl TypeInfo<Tp<T1, T2, T3...>>::info;

REGISTER_FUNDAMENTAL_TYPE(unsigned short, UnsignedShort)
REGISTER_FUNDAMENTAL_TYPE(unsigned int, UnsignedInt)
REGISTER_FUNDAMENTAL_TYPE(unsigned long long, UnsignedLongLong)
REGISTER_FUNDAMENTAL_TYPE(signed char, SignedChar)
REGISTER_FUNDAMENTAL_TYPE(short, Short)
REGISTER_FUNDAMENTAL_TYPE(int, Int)
REGISTER_FUNDAMENTAL_TYPE(long long, LongLong)
REGISTER_FUNDAMENTAL_TYPE(unsigned char, UnsignedChar)
REGISTER_FUNDAMENTAL_TYPE(char, Char)
REGISTER_FUNDAMENTAL_TYPE(wchar_t, Wchar_T)
REGISTER_FUNDAMENTAL_TYPE(long, Long)
REGISTER_FUNDAMENTAL_TYPE(bool, Bool)
REGISTER_FUNDAMENTAL_TYPE(unsigned long, UnsignedLong)
REGISTER_FUNDAMENTAL_TYPE(void, Void)
REGISTER_FUNDAMENTAL_TYPE(char16_t, Char16_T)
REGISTER_FUNDAMENTAL_TYPE(char32_t, Char32_T)
REGISTER_FUNDAMENTAL_TYPE(float, Float)
REGISTER_FUNDAMENTAL_TYPE(double, Double)
REGISTER_FUNDAMENTAL_TYPE(long double, LongDouble)
REGISTER_FUNDAMENTAL_TYPE(std::nullptr_t, STD_NullptrT)

//========================== TypeInfo with cv qualifier begin
//==========================

template<class Tp>
struct TypeInfo<const Tp> {
 private:
  class AnyImpl : public Any {
   public:
    MAKE_ID(TypeInfo<Tp>::info.id())
#define C "const"
    MAKE_NAME(decay()->PointerLevels() ? TypeInfo<Tp>::info.name() + C : C " " + TypeInfo<Tp>::info.name())
#undef C
    MAKE_DECAY(TypeInfo<Tp>::info.decay())
    MAKE_POINTER_LEVELS(decay()->PointerLevels())
    MAKE_IS_LVALUE_REFERENCE(TypeInfo<Tp>::info.IsLvalueReference())
    MAKE_IS_RVALUE_REFERENCE(TypeInfo<Tp>::info.IsRvalueReference())
    MAKE_IS_CONST(true)
    MAKE_SIZE(decay()->size())
    MAKE_FIRST(decay()->first())
    MAKE_SECOND(decay()->second())
  };

 public:
  static const AnyImpl info;
};
template<class Tp>
const typename TypeInfo<const Tp>::AnyImpl TypeInfo<const Tp>::info;

template<class Tp>
struct TypeInfo<volatile Tp> {
 private:
  class AnyImpl : public Any {
   public:
    MAKE_ID(TypeInfo<Tp>::info.id())
#define V "volatile"
    MAKE_NAME(decay()->PointerLevels() ? TypeInfo<Tp>::info.name() + V : V " " + TypeInfo<Tp>::info.name())
#undef V
    MAKE_DECAY(TypeInfo<Tp>::info.decay())
    MAKE_POINTER_LEVELS(decay()->PointerLevels())
    MAKE_IS_LVALUE_REFERENCE(TypeInfo<Tp>::info.IsLvalueReference())
    MAKE_IS_RVALUE_REFERENCE(TypeInfo<Tp>::info.IsRvalueReference())
    MAKE_IS_VOLATILE(true)
    MAKE_SIZE(decay()->size())
    MAKE_FIRST(decay()->first())
    MAKE_SECOND(decay()->second())
  };

 public:
  static const AnyImpl info;
};
template<class Tp>
const typename TypeInfo<volatile Tp>::AnyImpl TypeInfo<volatile Tp>::info;

template<class Tp>
struct TypeInfo<const volatile Tp> {
 private:
  class AnyImpl : public Any {
   public:
    MAKE_ID(TypeInfo<Tp>::info.id())
#define CV "const volatile"
    MAKE_NAME(decay()->PointerLevels() ? TypeInfo<Tp>::info.name() + CV : CV " " + TypeInfo<Tp>::info.name())
#undef CV
    MAKE_DECAY(TypeInfo<Tp>::info.decay())
    MAKE_POINTER_LEVELS(decay()->PointerLevels())
    MAKE_IS_LVALUE_REFERENCE(TypeInfo<Tp>::info.IsLvalueReference())
    MAKE_IS_RVALUE_REFERENCE(TypeInfo<Tp>::info.IsRvalueReference())
    MAKE_IS_VOLATILE(true)
    MAKE_IS_CONST(true)
    MAKE_SIZE(decay()->size())
    MAKE_FIRST(decay()->first())
    MAKE_SECOND(decay()->second())
  };

 public:
  static const AnyImpl info;
};
template<class Tp>
const typename TypeInfo<const volatile Tp>::AnyImpl TypeInfo<const volatile Tp>::info;

// ========================== TypeInfo with cv qualifier end
// ==========================

// ======================= TypeInfo with ptr and reference begin
// =======================

template<class Tp>
struct TypeInfo<Tp *> {
 private:
  class AnyImpl : public Any {
   public:
    MAKE_ID(TypeInfo<Tp>::info.id())
    MAKE_NAME(TypeInfo<Tp>::info.name() + "*")
    MAKE_POINTER_LEVELS(TypeInfo<Tp>::info.PointerLevels() + 1)
    MAKE_FIRST(&TypeInfo<Tp>::info)
  };

 public:
  static const AnyImpl info;
};
template<class Tp>
const typename TypeInfo<Tp *>::AnyImpl TypeInfo<Tp *>::info;

template<class Tp>
struct TypeInfo<Tp &> {
 private:
  class AnyImpl : public Any {
   public:
    MAKE_ID(TypeInfo<Tp>::info.id())
    MAKE_NAME(TypeInfo<Tp>::info.name() + "&")
    MAKE_DECAY(TypeInfo<Tp>::info.decay())
    MAKE_POINTER_LEVELS(decay()->PointerLevels())
    MAKE_IS_LVALUE_REFERENCE(true)
    MAKE_SIZE(decay()->size())
    MAKE_FIRST(decay()->first())
    MAKE_SECOND(decay()->second())
  };

 public:
  static const AnyImpl info;
};
template<class Tp>
const typename TypeInfo<Tp &>::AnyImpl TypeInfo<Tp &>::info;

template<class Tp>
struct TypeInfo<Tp &&> {
 private:
  class AnyImpl : public Any {
   public:
    MAKE_ID(TypeInfo<Tp>::info.id())
    MAKE_NAME(TypeInfo<Tp>::info.name() + "&&")
    MAKE_DECAY(TypeInfo<Tp>::info.decay())
    MAKE_POINTER_LEVELS(decay()->PointerLevels())
    MAKE_IS_RVALUE_REFERENCE(true)
    MAKE_SIZE(decay()->size())
    MAKE_FIRST(decay()->first())
    MAKE_SECOND(decay()->second())
  };

 public:
  static const AnyImpl info;
};
template<class Tp>
const typename TypeInfo<Tp &&>::AnyImpl TypeInfo<Tp &&>::info;

// ======================= TypeInfo with ptr and reference end
// =======================

// =============================== Array type begin
// ===============================

template<class Tp, size_t Size>
struct TypeInfo<Tp[Size]> {
 private:
  class AnyImpl : public Any {
   public:
    MAKE_ID(Array)
    MAKE_NAME(decay()->name() + ArraySizeName())
    MAKE_DECAY(TypeInfo<Tp>::info.decay())
    MAKE_SIZE(Size)
    MAKE_FIRST(&TypeInfo<Tp>::info)
    MAKE_ARRAY_SIZE_NAME("[" + std::to_string(Size) + "]" + TypeInfo<Tp>::info.ArraySizeName())
  };

 public:
  static const AnyImpl info;
};
template<class Tp, size_t Size>
const typename TypeInfo<Tp[Size]>::AnyImpl TypeInfo<Tp[Size]>::info;

template<class Tp, size_t Size>
struct TypeInfo<const Tp[Size]> {
 private:
  class AnyImpl : public Any {
   public:
    MAKE_ID(Array)
    MAKE_NAME("const " + TypeInfo<Tp[Size]>::info.name())
    MAKE_DECAY(TypeInfo<Tp>::info.decay())
    MAKE_IS_CONST(true);
    MAKE_SIZE(Size)
    MAKE_FIRST(TypeInfo<Tp[Size]>::info.first())
  };

 public:
  static const AnyImpl info;
};
template<class Tp, size_t Size>
const typename TypeInfo<const Tp[Size]>::AnyImpl TypeInfo<const Tp[Size]>::info;

template<class Tp, size_t Size>
struct TypeInfo<volatile Tp[Size]> {
 private:
  class AnyImpl : public Any {
   public:
    MAKE_ID(Array)
    MAKE_NAME("volatile " + TypeInfo<Tp[Size]>::info.name())
    MAKE_DECAY(TypeInfo<Tp>::info.decay())
    MAKE_IS_VOLATILE(true);
    MAKE_SIZE(Size)
    MAKE_FIRST(TypeInfo<Tp[Size]>::info.first())
  };

 public:
  static const AnyImpl info;
};
template<class Tp, size_t Size>
const typename TypeInfo<volatile Tp[Size]>::AnyImpl TypeInfo<volatile Tp[Size]>::info;

template<class Tp, size_t Size>
struct TypeInfo<const volatile Tp[Size]> {
 private:
  class AnyImpl : public Any {
   public:
    MAKE_ID(Array)
    MAKE_NAME("const volatile " + TypeInfo<Tp[Size]>::info.name())
    MAKE_DECAY(TypeInfo<Tp>::info.decay())
    MAKE_IS_CONST(true);
    MAKE_IS_VOLATILE(true);
    MAKE_SIZE(Size)
    MAKE_FIRST(TypeInfo<Tp[Size]>::info.first())
  };

 public:
  static const AnyImpl info;
};
template<class Tp, size_t Size>
const typename TypeInfo<const volatile Tp[Size]>::AnyImpl TypeInfo<const volatile Tp[Size]>::info;

// =============================== Array type end
// ===============================

template<>
struct TypeInfo<std::string> {
 private:
  class AnyImpl : public Any {
   public:
    MAKE_ID(STD_String)
    MAKE_NAME("std::string")
  };

 public:
  static const AnyImpl info;
};
const TypeInfo<std::string>::AnyImpl TypeInfo<std::string>::info;

template<class Tp, size_t Size>
struct TypeInfo<std::array<Tp, Size>> {
 private:
  class AnyImpl : public Any {
   public:
    MAKE_ID(STD_Array)
    MAKE_NAME("std::array<" + TypeInfo<Tp>::info.name() + "," + std::to_string(Size) + ">")
    MAKE_SIZE(Size)
    MAKE_FIRST(&TypeInfo<Tp>::info)
  };

 public:
  static const AnyImpl info;
};
template<class Tp, size_t Size>
const typename TypeInfo<std::array<Tp, Size>>::AnyImpl TypeInfo<std::array<Tp, Size>>::info;

REGISTER_CONTAINER(std::vector, STD_Vector)
REGISTER_CONTAINER(std::list, STD_List)
REGISTER_CONTAINER(std::stack, STD_Stack)
REGISTER_CONTAINER(std::queue, STD_Queue)
REGISTER_CONTAINER(std::deque, STD_Deque)
REGISTER_CONTAINER(std::priority_queue, STD_PriorityQueue)

REGISTER_CONTAINER(std::set, STD_Set)
REGISTER_CONTAINER(std::multiset, STD_MultiSet)
REGISTER_CONTAINER(std::unordered_set, STD_UnorderedSet)
REGISTER_CONTAINER(std::unordered_multiset, STD_UnorderedMultiset)

template<class T1, class T2>
struct TypeInfo<std::pair<T1, T2>> {
 private:
  class AnyImpl : public Any {
   public:
    MAKE_ID(STD_Pair)
    MAKE_NAME("std::pair<" + TypeInfo<T1>::info.name() + "," + TypeInfo<T2>::info.name() + ">")
    MAKE_FIRST(&TypeInfo<T1>::info)
    MAKE_SECOND(&TypeInfo<T2>::info)
  };

 public:
  static const AnyImpl info;
};
template<class T1, class T2>
const typename TypeInfo<std::pair<T1, T2>>::AnyImpl TypeInfo<std::pair<T1, T2>>::info;

REGISTER_PAIR_CONTAINER(std::map, STD_Map)
REGISTER_PAIR_CONTAINER(std::multimap, STD_Multimap)
REGISTER_PAIR_CONTAINER(std::unordered_map, STD_UnorderedMap)
REGISTER_PAIR_CONTAINER(std::unordered_multimap, STD_UnorderedMultimap)

}// namespace magic

#endif// MAGIC__NAME_OF_H_
