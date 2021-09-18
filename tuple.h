//
// Created by YorkDow Co on 2021/9/14.
//

#ifndef MAGIC__TUPLE_H_
#define MAGIC__TUPLE_H_

#include "index_sequence.h"

namespace magic {

template<size_t Index, class Tp>
struct TupleComponent {
  Tp value;
  TupleComponent() = default;
  explicit TupleComponent(Tp v) : value(v) {}
};

template<class Index, class ...Tp>
struct TupleImpl;

template<size_t ...IndexSeq, class ...Tp>
class TupleImpl<IndexSequence<IndexSeq...>, Tp...>
    : public TupleComponent<IndexSeq, Tp>... {

  template<size_t Begin, size_t Index, class Tp_, class ...Tps_>
  struct IterateElementType {
    static_assert(Index < sizeof...(IndexSeq), "index out of bound");
    using result = typename IterateElementType<Begin + 1, Index, Tps_...>::result;
  };

  template<size_t Index, class Tp_, class ...Tps_>
  struct IterateElementType<Index, Index, Tp_, Tps_...> {
    using result = Tp_;
  };

  template<size_t Index>
  struct GetElementType {
    using result = typename IterateElementType<0, Index, Tp...>::result;
  };

 public:
  TupleImpl() = default;

  template<class ...Up>
  explicit TupleImpl(Up ...up) : TupleComponent<IndexSeq, Tp>(up)... {}

  template<size_t Index>
  typename GetElementType<Index>::result &Get() {
    return TupleComponent<Index, typename GetElementType<Index>::result>::value;
  }

  template<class Fn>
  void ForEach(Fn &&func) {
    using expander = int[];
    (void) expander{0, (void(func(Get<IndexSeq>())), 0)...};
  }
};

template<class ...Tp>
class Tuple : public TupleImpl<typename IndexSeqImpl<0, sizeof...(Tp) - 1>::result, Tp...> {
 public:
  Tuple() = default;

  template<class ...Up>
  explicit Tuple(Up ...up) :
      TupleImpl<typename IndexSeqImpl<0, sizeof...(Tp) - 1>::result, Tp...>(up...) {}
};

template<class Tp>
struct TupleSize;

template<class ...Tp>
struct TupleSize<Tuple<Tp...>> {
  static const size_t result;
};

template<class ...Tp>
const size_t TupleSize<Tuple<Tp...>>::result = sizeof...(Tp);

template<class ...Tp>
inline Tuple<typename std::decay<Tp>::type...> MakeTuple(Tp &&...t) {
  return Tuple<typename std::decay<Tp>::type...>(std::forward<Tp>(t)...);
}

} // namespace magic

#endif // MAGIC__TUPLE_H_
