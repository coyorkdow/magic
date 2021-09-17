//
// Created by YorkDow Co on 2021/6/7.
//

#ifndef MAGIC__QUICK_POWER_H_
#define MAGIC__QUICK_POWER_H_

#include "util.h"

namespace magic {

template <uint64_t Num> struct MaxDigit { static const uint64_t result; };
template <uint64_t Num>
const uint64_t MaxDigit<Num>::result = 1 + MaxDigit<(Num >> 1)>::result;
template <> const uint64_t MaxDigit<0>::result = 0;

template <uint64_t Base, uint64_t EXP, uint64_t MOD> class Power {

  template <uint64_t A, uint64_t B> struct Times {
    static const uint64_t result = A * B % MOD;
  };

  template <uint64_t DIGIT, uint64_t Current> struct Process {
    using Next = Process<DIGIT << 1, Current * Current % MOD>;
    static const uint64_t result =
        IfThenElse<bool(DIGIT &EXP), Times<Current, Next::result>,
                   Times<1, Next::result>>::Result::result;
  };

  template <uint64_t Current>
  struct Process<(1 << (MaxDigit<EXP>::result - 1)), Current> {
    static const uint64_t result = Current % MOD;
  };

public:
  inline constexpr static uint64_t Calculate() {
    return Process<1, Base>::result;
  }
};

} // namespace magic

#endif // MAGIC__QUICK_POWER_H_
