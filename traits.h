//
// Created by YorkDow Co on 2021/12/21.
//

#ifndef MAGIC__TRAITS_H_
#define MAGIC__TRAITS_H_

#include <iterator>

namespace magic {

namespace detail {
using std::begin;
using std::declval;
using std::end;

template<class T, class = void>
struct BeginType;

template<class T>
struct BeginType<T, decltype(begin(declval<T>()), ++begin(declval<T>()), (void) *begin(declval<T>()))> {
  using type = decltype(begin(declval<T>()));
};

template<class T, class = void>
struct EndType;

template<class T>
struct EndType<T, decltype(end(declval<T>()), ++end(declval<T>()), (void) *end(declval<T>()))> {
  using type = decltype(begin(declval<T>()));
};

template<class T, class... Args>
auto IsConstructibleImpl(int) -> decltype(T(declval<Args>()...), std::true_type{});

template<class T, class...>
auto IsConstructibleImpl(...) -> std::false_type;

}// namespace detail

template<class T, class = void>
struct IsIterable : public std::false_type {};

template<class T>
struct IsIterable<T,
                  typename std::enable_if<std::is_same<typename detail::BeginType<T>::type,
                                                       typename detail::EndType<T>::type>{},
                                          void>::type>
    : public std::true_type {};

template<class T, class... Args>
struct IsConstructible : public std::integral_constant<bool, decltype(detail::IsConstructibleImpl<T, Args...>(0))::value> {};

}// namespace magic

#endif//MAGIC__TRAITS_H_
