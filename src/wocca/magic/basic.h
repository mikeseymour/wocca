#pragma once
namespace wocca {
namespace magic {

// Wrap an alias template as a nested type in a class template. This is more
// convenient when defining complex types:
//  * it allows partial specialisation
//  * it prevents unwanted instantiation (e.g. the "false" case of "conditional")
//  * the type can be inherited from other wrappers
//
// Wrapped class templates are usually named with a trailing underscore.
template <class T> struct wrap {using type = T;};
template <class T> using unwrap = typename T::type;

// Choose a type depending on a condition.
template <bool, class T1, class> struct conditional_ : wrap<T1> {};
template <class T1, class T2> struct conditional_<false,T1,T2> : wrap<T2> {};
template <bool C, class T1, class T2> using conditional = unwrap<conditional_<C,T1,T2>>;

// Compare two types
template <class, class> constexpr bool same = false;
template <class T> constexpr bool same<T,T> = true;

}}

