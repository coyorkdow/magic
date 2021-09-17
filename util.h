//
// Created by YorkDow Co on 2021/6/7.
//

#ifndef MAGIC__UTIL_H_
#define MAGIC__UTIL_H_

namespace magic {

template<bool Cond, class Then, class Else>
struct IfThenElse;

template<class Then, class Else>
struct IfThenElse<true, Then, Else> {
  using Result = Then;
};

template<class Then, class Else>
struct IfThenElse<false, Then, Else> {
  using Result = Else;
};

}

#endif // MAGIC__UTIL_H_
