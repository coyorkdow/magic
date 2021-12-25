//
// Created by YorkDow Co on 2021/9/14.
//

#ifndef MAGIC__TUPLE_H_
#define MAGIC__TUPLE_H_

#include <type_traits>
#include <utility>

#include "index_sequence.h"
#include "util.h"

namespace magic {

template<size_t Index, class Tp>
struct TupleComponent {
 protected:
  Tp value;
  constexpr TupleComponent() = default;
  constexpr explicit TupleComponent(Tp v) : value(v) {}
};

template<class Index, class... Tp>
class TupleImpl;

template<size_t... IndexSeq, class... Tp>
class TupleImpl<IndexSequence<IndexSeq...>, Tp...>
    : private TupleComponent<IndexSeq, Tp>... {

  template<size_t Begin, size_t Index, class Tp_, class... Tps_>
  struct IterateElementType {
    static_assert(Index < sizeof...(IndexSeq), "index out of bound");
    using result = typename IterateElementType<Begin + 1, Index, Tps_...>::result;
  };

  template<size_t Index, class Tp_, class... Tps_>
  struct IterateElementType<Index, Index, Tp_, Tps_...> {
    using result = Tp_;
  };

  template<size_t Index>
  struct GetElementType {
    using result = typename IterateElementType<0, Index, Tp...>::result;
  };

 protected:
  constexpr TupleImpl() = default;

  template<class... Up>
  constexpr explicit TupleImpl(Up... up) : TupleComponent<IndexSeq, Tp>(up)... {}

  template<size_t Index>
  const typename GetElementType<Index>::result &Get() const {
    return TupleComponent<Index, typename GetElementType<Index>::result>::value;
  }

  template<size_t Index>
  void Set(const typename GetElementType<Index>::result &val) {
    TupleComponent<Index, typename GetElementType<Index>::result>::value = val;
  }

  template<class Fn>
  void ForEach(Fn &&func) {
    using expander = int[];
    (void) expander{(void(func(Get<IndexSeq>())), 0)...};
  }
};

template<class... Tp>
class Tuple : protected TupleImpl<typename IndexSeqImpl<0, sizeof...(Tp) - 1>::result, Tp...> {
  using Base = TupleImpl<typename IndexSeqImpl<0, sizeof...(Tp) - 1>::result, Tp...>;

 public:
  using Base::ForEach;
  using Base::Get;
  using Base::Set;

  constexpr Tuple() = default;

  template<class... Up>
  constexpr explicit Tuple(Up... up) : Base(up...) {}

  inline constexpr size_t size() const noexcept { return sizeof...(Tp); }
};

template<class Tp>
struct TupleSize;

template<class... Tp>
struct TupleSize<Tuple<Tp...>> {
  static const size_t size = sizeof...(Tp);
};

template<class... Tp>
inline constexpr Tuple<typename std::decay<Tp>::type...> MakeTuple(Tp &&...t) {
  return Tuple<typename std::decay<Tp>::type...>(std::forward<Tp>(t)...);
}

}// namespace magic

#endif// MAGIC__TUPLE_H_
