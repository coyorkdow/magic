//
// Created by YorkDow Co on 2021/9/17.
//

#ifndef MAGIC__REFLECTION_H_
#define MAGIC__REFLECTION_H_

#include <cassert>
#include <functional>
#include <type_traits>

#include "traits.h"
#include "tuple.h"
#include "type_of.h"

namespace magic {

template <class T>
using decay_t = typename std::decay<T>::type;

template <class T>
struct IsReflectableType : public std::false_type {};

template <class Tp>
class TypeFieldsScheme;

#define REGISTER_FIELDS(Tp, ...)                                               \
  namespace magic {                                                            \
  template <>                                                                  \
  class TypeFieldsScheme<Tp> {                                                 \
    using Tp_ = Tp;                                                            \
    using rT_ = std::remove_reference<decltype(MakeTuple(__VA_ARGS__))>::type; \
                                                                               \
   public:                                                                     \
    static constexpr rT_ result() { return MakeTuple(__VA_ARGS__); }           \
    static constexpr auto name = #Tp;                                          \
    static constexpr size_t size = MakeTuple(__VA_ARGS__).size();              \
  };                                                                           \
                                                                               \
  template <>                                                                  \
  struct IsReflectableType<Tp> : public std::true_type {};                     \
  }

#define FIELD(var, tag) MakeTuple(&Tp_::var, #var, #tag)

#define FIELD_NO_TAG(var) MakeTuple(&Tp_::var, #var, "")

#define IS_REFLECTABLE(val) IsReflectableType<decay_t<decltype(val)>>()

using UnifiedField = Tuple<size_t, std::string, std::string, const TypeInfoT*>;

template <class Tp>
class AllFields {
 public:
  void* PtrOf(size_t ind) const {
    assert(ind < size());
    return reinterpret_cast<char*>(&val_) + res_[ind].template Get<0>();
  }

  const std::string& NameOf(size_t ind) const {
    assert(ind < size());
    return res_[ind].template Get<1>();
  }

  const std::string& TagOf(size_t ind) const {
    assert(ind < size());
    return res_[ind].template Get<2>();
  }

  const TypeInfoT* TypeOf(size_t ind) const {
    assert(ind < size());
    return res_[ind].template Get<3>();
  }

  template <class Val>
  void Set(size_t ind, Val&& v) {
    assert(ind < size());
    using ValTp = decay_t<Val>;
    auto ptr = reinterpret_cast<ValTp*>(reinterpret_cast<char*>(&val_) + res_[ind].template Get<0>());
    *ptr = std::forward<Val>(v);
  }

  inline constexpr size_t size() const noexcept { return AllFields<Tp>::size_; }

 private:
  template <class T_>
  friend AllFields<decay_t<T_>> GetAllFields(T_&&);

  explicit AllFields(Tp& val) : val_(val) {}

  static constexpr size_t size_ = TypeFieldsScheme<Tp>::result().size();

  using UnifiedFields = UnifiedField[size_];

  template <class TupleField>
  static UnifiedField HandleField(TupleField&& field) {
    Tp* base = nullptr;
    size_t offset = (char*)&(base->*(std::forward<TupleField>(field).template Get<0>())) - (char*)base;
    std::string name = std::forward<TupleField>(field).template Get<1>();
    std::string tag = std::forward<TupleField>(field).template Get<2>();
    using rT = decltype(base->*(std::forward<TupleField>(field).template Get<0>()));
    const TypeInfoT* meta = &TypeInfo<typename std::remove_reference<rT>::type>::info();
    return MakeTuple(offset, std::move(name), std::move(tag), meta);
  }

  template <size_t... N>
  static const UnifiedFields& Impl(IndexSequence<N...>) {
    static const auto scheme = TypeFieldsScheme<Tp>::result();
    static const UnifiedFields expander = {HandleField(scheme.template Get<N>())...};
    return expander;
  }

  static const UnifiedFields& res_;

  Tp& val_;
};

template <class Tp>
const typename AllFields<Tp>::UnifiedFields& AllFields<Tp>::res_ =
    AllFields<Tp>::Impl(MakeIndexSequence<TypeFieldsScheme<Tp>::result().size()>());

template <class Tp>
AllFields<decay_t<Tp>> GetAllFields(Tp&& val) {
  return AllFields<decay_t<Tp>>(std::forward<Tp>(val));
}

template <size_t Index, class T>
auto GetField(T&& var) -> decltype(TypeFieldsScheme<decay_t<T>>::result().template Get<Index>()) {
  return TypeFieldsScheme<decay_t<T>>::result().template Get<Index>();
}

template <size_t Index, class T>
auto ValueOf(T&& val) -> decltype(val.*GetField<Index>(std::forward<T>(val)).template Get<0>()) {
  return val.*GetField<Index>(std::forward<T>(val)).template Get<0>();
}

template <size_t Index, class T>
auto NameOf(T&& val) -> decltype(GetField<Index>(std::forward<T>(val)).template Get<1>()) {
  return GetField<Index>(std::forward<T>(val)).template Get<1>();
}

template <size_t Index, class T>
std::string TypeNameOf(T&& val) {
  using rT = decltype(val.*GetField<Index>(std::forward<T>(val)).template Get<0>());
  return TypeInfo<typename std::remove_reference<rT>::type>::info().name();
}

template <size_t Index, class T>
auto TagOf(T&& val) -> decltype(GetField<Index>(std::forward<T>(val)).template Get<2>()) {
  return GetField<Index>(std::forward<T>(val)).template Get<2>();
}

template <class T>
auto NameOf(T&&) -> decltype(TypeFieldsScheme<decay_t<T>>::name) {
  return TypeFieldsScheme<decay_t<T>>::name;
}

template <class Tp, class Fn>
class MakeHandler {
 public:
  template <class Fn_>
  explicit MakeHandler(Tp& v, Fn_&& fn) : var_(v), fn(std::forward<Fn_>(fn)) {}

  template <class Field>
  void operator()(Field&& field) {
    //    static_assert(typename std::decay<Field>::type().size() == 3,
    //                  "invalid argument, not a Filed");
    static_assert(TupleSize<decay_t<Field>>::size == 3, "invalid argument, not a Filed");
    fn(var_.*std::forward<Field>(field).template Get<0>(), std::forward<Field>(field).template Get<1>(),
       std::forward<Field>(field).template Get<2>());
  }

 private:
  Tp& var_;
  Fn fn;
};

template <class Handler>
class ForEachField {
 public:
  template <class T, class... Args>
  typename std::enable_if<IsConstructible<Handler, Args...>{}>::type Iterate(T&& var, Args&&... args) {
    static auto scheme = TypeFieldsScheme<decay_t<T>>::result();
    scheme.ForEach(MakeHandler<T, Handler>(var, Handler(std::forward<Args>(args)...)));
  };
};

}  // namespace magic

#endif  // MAGIC__REFLECTION_H_
