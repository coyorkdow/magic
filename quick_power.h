//
// Created by YorkDow Co on 2021/6/7.
//

#ifndef MAGIC__QUICK_POWER_H_
#define MAGIC__QUICK_POWER_H_

#include "util.h"

namespace magic {

template<uint64_t Num>
struct MaxDigit {
  static const uint64_t result = 1 + MaxDigit<(Num >> 1)>::result;
};

template<>
struct MaxDigit<0> {
  static const uint64_t result = 0;
};

template<uint64_t Base, uint64_t Exp, uint64_t Mod>
class Power {

  template<uint64_t A, uint64_t B>
  struct Times {
    static const uint64_t result = A * B % Mod;
  };

  template<uint64_t DIGIT, uint64_t Current>
  struct Process {
    using next = Process<DIGIT << 1, Current * Current % Mod>;
    static const uint64_t result =
        IfThenElse<bool(DIGIT& Exp), Times<Current, next::result>,
                   Times<1, next::result>>::result::result;
  };

  template<uint64_t Current>
  struct Process<(1 << (MaxDigit<Exp>::result - 1)), Current> {
    static const uint64_t result = Current % Mod;
  };

 public:
  inline constexpr static uint64_t Calculate() {
    return Process<1, Base>::result;
  }
};

}// namespace magic

#endif// MAGIC__QUICK_POWER_H_
