//
// Created by YorkDow Co on 2021/12/22.
//

#ifndef MAGIC_EXAMPLES_EXAMPLES_UTIL_H_
#define MAGIC_EXAMPLES_EXAMPLES_UTIL_H_

#include <iostream>

#include "traits.h"

// Override std::ostream so that std::cout could print stl container directly.
template <class T1, class T2>
inline std::ostream& operator<<(std::ostream& os, const std::pair<T1, T2>& v) {
  os << '{' << v.first << ", " << v.second << '}';
  return os;
}

template <class ContainerT>
typename std::enable_if<magic::IsIterable<ContainerT>{} && !std::is_same<ContainerT, std::string>{},
                        std::ostream>::type&
operator<<(std::ostream& os, const ContainerT& container) {
  using std::begin;
  using std::end;
  os << '[';
  auto it = begin(container), ed = end(container);
  if (it != ed) {
    os << *it;
    while (++it != ed) {
      os << ' ' << *it;
    }
  }
  os << ']';
  return os;
}

#endif  // MAGIC_EXAMPLES_EXAMPLES_UTIL_H_
