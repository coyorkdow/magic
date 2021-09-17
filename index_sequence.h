//
// Created by YorkDow Co on 2021/9/14.
//

#ifndef MAGIC__INDEX_SEQUENCE_H_
#define MAGIC__INDEX_SEQUENCE_H_

namespace magic {

template <size_t...> struct IndexSequence {};

template <class, class> struct IndexSeqCombiner;

template <size_t... Left, size_t... Right>
struct IndexSeqCombiner<IndexSequence<Left...>, IndexSequence<Right...>> {
  using result = IndexSequence<Left..., Right...>;
};

template <size_t Begin, size_t End> struct IndexSeqImpl {
#define Mid (Begin + ((End - Begin) >> 1))
  using LeftPart = typename IndexSeqImpl<Begin, Mid>::result;
  using RightPart = typename IndexSeqImpl<Mid + 1, End>::result;
#undef Mid

  using result = typename IndexSeqCombiner<LeftPart, RightPart>::result;
};

template <size_t Begin> struct IndexSeqImpl<Begin, Begin> {
  using result = IndexSequence<Begin>;
};

template <size_t N>
using MakeIndexSequence = typename IndexSeqImpl<0, N - 1>::result;

} // namespace magic

#endif // MAGIC__INDEX_SEQUENCE_H_
