//
// Created by YorkDow Co on 2021/9/14.
//

#ifndef MAGIC__TUPLE_H_
#define MAGIC__TUPLE_H_

#include "index_sequence.h"

namespace magic {

template <size_t Index, typename Tp> struct TupleComponent {
  Tp value;
  TupleComponent() = default;
  explicit TupleComponent(Tp v) : value(v) {}
};

template <class Index, typename... Tp> struct TupleImpl;

template <size_t... IndexSeq, typename... Tp>
class TupleImpl<IndexSequence<IndexSeq...>, Tp...>
    : public TupleComponent<IndexSeq, Tp>... {

  template <size_t Begin, size_t Index, typename Tp_, typename... Tps_>
  struct IterateElementType {
    static_assert(Index < sizeof...(IndexSeq), "index out of bound");
    using result =
        typename IterateElementType<Begin + 1, Index, Tps_...>::result;
  };

  template <size_t Index, typename Tp_, typename... Tps_>
  struct IterateElementType<Index, Index, Tp_, Tps_...> {
    using result = Tp_;
  };

  template <size_t Index> struct GetElementType {
    using result = typename IterateElementType<0, Index, Tp...>::result;
  };

public:
  TupleImpl() = default;

  template <typename... Up>
  explicit TupleImpl(Up... up) : TupleComponent<IndexSeq, Tp>(up)... {}

  template <size_t Index> typename GetElementType<Index>::result &Get() {
    return TupleComponent<Index, typename GetElementType<Index>::result>::value;
  }
};

template <typename... Tp>
using Tuple = TupleImpl<MakeIndexSequence<sizeof...(Tp)>, Tp...>;

template <class... Tp>
inline Tuple<typename std::decay<Tp>::type...> MakeTuple(Tp &&...t) {
  return Tuple<typename std::decay<Tp>::type...>(std::forward<Tp>(t)...);
}

} // namespace magic

#endif // MAGIC__TUPLE_H_
