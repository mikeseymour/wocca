#pragma once
namespace wocca {
namespace magic {

// Choose a type depending on a condition.
template <bool, class T1, class> struct conditional_ {using type = T1;};
template <class T1, class T2> struct conditional_<false,T1,T2> {using type = T2;};
template <bool C, class T1, class T2> using conditional = typename conditional_<C,T1,T2>::type;

// Compare two types
template <class, class> constexpr bool same = false;
template <class T> constexpr bool same<T,T> = true;

}}

