//
// Created by YorkDow Co on 2021/6/7.
//

#pragma once

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

#define IF_THEN_ELSE(cond_, then_, else_) typename IfThenElse<cond_, then_, else_>::Result
