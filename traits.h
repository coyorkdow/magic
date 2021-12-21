//
// Created by YorkDow Co on 2021/12/21.
//

#ifndef MAGIC__TRAITS_H_
#define MAGIC__TRAITS_H_

#include <iterator>

namespace magic {

namespace detail {
using std::begin;
using std::end;

template<class T, class = void>
struct BeginType;

template<class T>
struct BeginType<T, decltype(begin(std::declval<T>()), ++begin(std::declval<T>()), (void) *begin(std::declval<T>()))> {
  using type = decltype(begin(std::declval<T>()));
};

template<class T, class = void>
struct EndType;

template<class T>
struct EndType<T, decltype(end(std::declval<T>()), ++end(std::declval<T>()), (void) *end(std::declval<T>()))> {
  using type = decltype(begin(std::declval<T>()));
};

}// namespace detail

template<class T, class = void>
struct IsIterable : public std::false_type {};

template<class T>
struct IsIterable<T,
                  typename std::enable_if<std::is_same<typename detail::BeginType<T>::type,
                                                       typename detail::EndType<T>::type>{},
                                          void>::type>
    : public std::true_type {};

}// namespace magic

#endif//MAGIC__TRAITS_H_
