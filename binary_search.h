//
// Created by YorkDow Co on 2021/6/10.
//

#pragma once

#include "util.h"

template<typename T>
struct Comparator {
  static constexpr bool Result(const T &_l, const T &_r) {
    return _l < _r;
  }
};

template<typename T, class Comp = Comparator<T>>
class BinarySearch {

//  template<Ptr Begin, Ptr End, T Value>
//  struct __LowerBound {
//    using mid = Mid<Begin, End>;
//    using less = typename Comp::template Less<Value, *mid::result>;
//    static const Ptr result = IfThenElse<less::result,
//                                         __LowerBound<Begin + 1, mid::result, Value>,
//                                         __LowerBound<mid::result, End, Value>>::Result::result;
//  };
//  template<typename Pointer<T>::Result End, T Value>
//  struct __LowerBound<End, End, Value> {
//    static const typename Pointer<T>::Result result = End;
//  };

  struct Less {
    static constexpr bool Result(const T &_l, const T &_r) {
      return Comp::Result(_l, _r);
    }
  };
  struct GreaterOrEqual {
    static constexpr bool Result(const T &_l, const T &_r) {
      return !Less::Result(_r, _l);
    }
  };

 public:
  template<std::size_t N>
  static constexpr size_t LowerBound(T const (&array)[N], T value) {
    return PerformSearch(array, value, 0, N, Less::Result);
  }

  template<std::size_t N>
  static constexpr size_t UpperBound(T const (&array)[N], T value) {
    return PerformSearch(array, value, 0, N, GreaterOrEqual::Result);
  }

 private:
#define MID (l + ((r - l) >> 1))
  template<std::size_t N, class Compare>
  static constexpr size_t PerformSearch
      (T const (&array)[N], T value, size_t l, size_t r, Compare value_compare) {
    return (l == r) ? l
                    : value_compare(array[MID], value)
                      ? PerformSearch(array, value, MID + 1, r, value_compare)
                      : PerformSearch(array, value, l, MID, value_compare);
  };
#undef MID
};
