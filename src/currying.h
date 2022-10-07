//
// Created by YorkDow Co on 2022/10/7.
//

#ifndef MAGIC_CURRYING_H
#define MAGIC_CURRYING_H

#define TP_(arg) typename arg
#define CONCAT_(...) __VA_ARGS__

#define CURRY_(N, _1, params, args)                           \
  template <template <_1, params> class C, typename FirstArg> \
  struct Curry##N {                                           \
    template <params>                                         \
    using result = C<FirstArg, args>;                         \
  };                                                          \
  template <template <_1, params> class C, typename Arg>      \
  constexpr auto Curry()->Curry##N<C, Arg>;

template <template <typename _1> class C, typename FirstArg>
struct Curry1 {
  using result = C<FirstArg>;
};
template <template <typename _1> class C, typename Arg>
constexpr auto Curry() -> Curry1<C, Arg>;

CURRY_(2, TP_(_1), TP_(_2), _2)
CURRY_(3, TP_(_1), CONCAT_(TP_(_2), TP_(_3)), CONCAT_(_2, _3))
CURRY_(4, TP_(_1), CONCAT_(TP_(_2), TP_(_3), TP_(_4)), CONCAT_(_2, _3, _4))
CURRY_(5, TP_(_1), CONCAT_(TP_(_2), TP_(_3), TP_(_4), TP_(_5)), CONCAT_(_2, _3, _4, _5))
CURRY_(6, TP_(_1), CONCAT_(TP_(_2), TP_(_3), TP_(_4), TP_(_5), TP_(_6)), CONCAT_(_2, _3, _4, _5, _6))
CURRY_(7, TP_(_1), CONCAT_(TP_(_2), TP_(_3), TP_(_4), TP_(_5), TP_(_6), TP_(_7)), CONCAT_(_2, _3, _4, _5, _6, _7))
CURRY_(8, TP_(_1), CONCAT_(TP_(_2), TP_(_3), TP_(_4), TP_(_5), TP_(_6), TP_(_7), TP_(_8)),
       CONCAT_(_2, _3, _4, _5, _6, _7, _8))
CURRY_(9, TP_(_1), CONCAT_(TP_(_2), TP_(_3), TP_(_4), TP_(_5), TP_(_6), TP_(_7), TP_(_8), TP_(_9)),
       CONCAT_(_2, _3, _4, _5, _6, _7, _8, _9))
CURRY_(10, TP_(_1), CONCAT_(TP_(_2), TP_(_3), TP_(_4), TP_(_5), TP_(_6), TP_(_7), TP_(_8), TP_(_9), TP_(_10)),
       CONCAT_(_2, _3, _4, _5, _6, _7, _8, _9, _10))

#define CURRY(Class, Arg) decltype(Curry<Class, Arg>())::result

#endif  // MAGIC_CURRYING_H
