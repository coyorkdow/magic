//
// Created by YorkDow Co on 2021/6/10.
//

#ifndef MAGIC__BINARY_SEARCH_H_
#define MAGIC__BINARY_SEARCH_H_

#include "util.h"

namespace magic {

template<class T>
struct Comparator {
  static constexpr bool Result(const T &_l, const T &_r) { return _l < _r; }
};

template<class T, class Comp = Comparator<T>>
class BinarySearch {

  struct Less {
    static constexpr bool Result(const T &_l, const T &_r) {
      return Comp::Result(_l, _r);
    }
  };

  struct LessOrEqual {
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
    return PerformSearch(array, value, 0, N, LessOrEqual::Result);
  }

 private:
#define MID (l + ((r - l) >> 1))
  template<std::size_t N, class Compare>
  static constexpr size_t PerformSearch(T const (&array)[N], T value, size_t l,
                                        size_t r, Compare value_compare) {
    return (l == r) ? l
                    : value_compare(array[MID], value)
                      ? PerformSearch(array, value, MID + 1, r, value_compare)
                      : PerformSearch(array, value, l, MID, value_compare);
  };
#undef MID
};

} // namespace magic

#endif // MAGIC__BINARY_SEARCH_H_
