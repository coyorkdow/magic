//
// Created by YorkDow Co on 2021/9/17.
//

#ifndef MAGIC__REFLECTION_H_
#define MAGIC__REFLECTION_H_

#include <functional>
#include <type_traits>

#include "tuple.h"

namespace magic {

template<class Tp>
struct TypeFieldsScheme;

#define RegisterFields(Tp, ...)                                                \
  template <> struct TypeFieldsScheme<Tp> {                                    \
    using Tp_ = Tp;                                                            \
    static constexpr auto result() -> decltype(MakeTuple(__VA_ARGS__)) {       \
      return MakeTuple(__VA_ARGS__);                                           \
    };                                                                         \
  };

#define Field(var, type_name) MakeTuple(&Tp_::var, #var, #type_name)

template<class Tp>
class GetFields {
  using UnifiedField = Tuple<size_t, std::string, std::string>;
  using ThisFields = UnifiedField[TupleSize<decltype(TypeFieldsScheme<Tp>::result())>::result];

  template<class Field>
  static UnifiedField HandleField(Field &&field) {
    Tp *base = nullptr;
    size_t offset = (char *) &(base->*(std::forward<Field>(field).template Get<0>())) - (char *) base;
    std::string name = std::forward<Field>(field).template Get<1>();
    std::string type_name = std::forward<Field>(field).template Get<2>();
    return MakeTuple(offset, std::move(name), std::move(type_name));
  }

  template<size_t... N>
  static const ThisFields& Impl(IndexSequence<N...>) {
    static auto scheme = TypeFieldsScheme<Tp>::result();
    static const ThisFields expander = {HandleField(scheme.template Get<N>())...};
    return expander;
  }

 public:
  static const ThisFields& result() {
    return Impl(MakeIndexSequence<TupleSize<decltype(TypeFieldsScheme<Tp>::result())>::result>());
  }
};

template<class Tp, class Fn>
class MakeHandler {
 public:
  explicit MakeHandler(const Tp &v) : var_(v) {}

  template<class Field>
  void operator()(Field &&field) {
    static_assert(TupleSize<typename std::decay<Field>::type>::result == 3,
                  "invalid argument, not a Filed");
    Fn()(var_.*(std::forward<Field>(field).template Get<0>()),
         std::forward<Field>(field).template Get<1>(),
         std::forward<Field>(field).template Get<2>());
  }

 private:
  const Tp &var_;
};

template<class Handler>
class ForEachField {
 public:
  template<class T>
  void operator()(T &&var) {
    using T_ = typename std::decay<T>::type;
    auto scheme = TypeFieldsScheme<T_>::result();
    scheme.ForEach(MakeHandler<T_, Handler>(static_cast<T_>(var)));
  };
};

} // namespace magic

#endif // MAGIC__REFLECTION_H_