#ifndef WOCCA_MAGIC_TUPLE_H
#define WOCCA_MAGIC_TUPLE_H

#include "wocca/magic/basic.h"

#include <cstddef>
#include <type_traits>
#include <tuple>

// Functions and metafunctions for dealing with tuples

namespace wocca {
namespace magic {
namespace tuple {

// Make a tuple from these types
template <typename... Ts> using make = meta<std::tuple<Ts...>>;
typedef make<> empty;

// Determine whether a tuple contains a type
template <typename T, typename Tuple> struct contains : contains<T, self<Tuple>> {};
template <typename T, typename... Ts> struct contains<T, make<T,Ts...>> : std::true_type {};
template <typename T>                 struct contains<T, empty> : std::false_type {};
template <typename T, typename H, typename... Ts>
    struct contains<T, make<H,Ts...>> : contains<T,make<Ts...>> {};

// Determine whether a tuple contains two types, the first before the second
template <typename T1, typename T2, typename Tuple> struct before : before<T1, T2, self<Tuple>> {};
template <typename T1,              typename Tuple> struct before<T1, T1, Tuple>          : std::false_type {};
template <typename T1, typename T2, typename... Ts> struct before<T1, T2, make<T1,Ts...>> : contains<T2, make<Ts...>> {};
template <typename T1, typename T2, typename... Ts> struct before<T1, T2, make<T2,Ts...>> : std::false_type {};
template <typename T1, typename T2>                 struct before<T1, T2, empty>          : std::false_type {};
template <typename T1, typename T2, typename H, typename... Ts>
    struct before<T1, T2, make<H,Ts...>> : before<T1, T2, make<Ts...>> {};

// Add a type to the start of a tuple, optionally specifying a condition
template <typename T, typename Tuple, bool Condition = true> struct prepend : prepend<T, self<Tuple>, Condition> {};
template <typename T, typename Tuple> struct prepend<T, Tuple,       false> : self<Tuple> {};
template <typename T, typename... Ts> struct prepend<T, make<Ts...>, true>  : make<T,Ts...> {};

// Add a type to the start of a tuple if it's not already in the tuple
template <typename T, typename Tuple> struct prepend_unique :
    prepend<T, Tuple, !contains<T, Tuple>()> {};

// Tuple size, for value or reference types
template <typename Tuple> struct size :
    std::tuple_size<typename std::decay<Tuple>::type> {};

// Iterate over all elements of a tuple instance
template <typename Tuple, typename Functor, bool Forward=true, size_t I=size<Tuple>()>
typename std::enable_if<I == 0>::type
visit(Tuple &&, Functor &&) {}

template <typename Tuple, typename Functor, bool Forward=true, size_t I=size<Tuple>()>
typename std::enable_if<I != 0>::type
visit(Tuple && t, Functor && f) {
    f(std::get<Forward ? size<Tuple>()-I : I-1>(t));
    visit<Tuple, Functor, Forward, I-1>(std::forward<Tuple>(t), std::forward<Functor>(f));
}

// Iterate backwards over all elements of a tuple instance
template <typename Tuple, typename Functor>
void rvisit(Tuple && t, Functor f) {
    visit<Tuple,Functor,false>(std::forward<Tuple>(t), std::forward<Functor>(f));
}

}}}
#endif
