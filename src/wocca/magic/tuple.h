#pragma once
#include "wocca/magic/basic.h"

namespace wocca {
namespace magic {

template <class... Ts> struct tuple {
    tuple(Ts...) {}

    template <class F> void visit(F &&) {}
    template <class F> void rvisit(F &&) {}
};

template <class Head, class... Tail>
struct tuple<Head, Tail...> {
    tuple() {}
    tuple(Head head, Tail... tail) : head(head), tail(tail...) {}

    Head head;
    tuple<Tail...> tail;

    template <class F> void visit(F && f) {f(head); tail.visit(f);}
    template <class F> void rvisit(F && f) {tail.rvisit(f); f(head);}
};

namespace tuples {
    // The size of a tuple
    template <class Tuple> constexpr unsigned size = 0;
    template <typename... Ts> constexpr unsigned size<tuple<Ts...>> = sizeof...(Ts);

    // Check whether a type appears in a tuple
    template <class T, class Tuple> constexpr bool contains = false;
    template <class T, class H, class... Ts> 
        constexpr bool contains<T,tuple<H,Ts...>> = 
            same<T,H> || contains<T,tuple<Ts...>>;

    // Check whether two types appear in a tuple in the specified order
    template <class T1, class T2, class Tuple> constexpr bool before = false;
    template <class T1, class T2, class H, class... Ts> 
        constexpr bool before<T1,T2,tuple<H,Ts...>> = 
            (same<T1,H> && contains<T2,tuple<Ts...>>) || 
                before<T1,T2,tuple<Ts...>>;

    // Prepend a type to a tuple
    template <class T, class Tuple> struct prepend_;
    template <class T, class... Ts> struct prepend_<T,tuple<Ts...>> {using type = tuple<T,Ts...>;};
    template <class T, class Tuple> using prepend = typename prepend_<T,Tuple>::type;
}

}}

