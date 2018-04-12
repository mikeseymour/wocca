#pragma once
#include <array>

namespace wocca {
namespace tags {

template <int... Keys> struct keyset {
	static constexpr int keys[] {Keys...};
};

template <int Key, class Keys> struct prepend_;
template <int Key, class Keys> using prepend = typename prepend_<Key, Keys>::result;

template <int Key, class Keys> 
struct prepend_ {
	using result = keyset<Key>;
};
template <int Key, int... Keys> 
struct prepend_<Key, keyset<Keys...>> {
	using result = keyset<Key, Keys...>; 
};

template <int Key, class Keys> struct remove_;
template <int Key, class Keys> using remove = typename remove_<Key, Keys>::result;

template <int Key, class Keys> 
struct remove_ {
	using result = keyset<>;
};
template <int Key, int... Tail>
struct remove_<Key, keyset<Key, Tail...>> {
	using result = keyset<Tail...>;
};
template <int Key, int Head, int... Tail>
struct remove_<Key, keyset<Head, Tail...>> {
	using result = prepend<Head, remove<Key, keyset<Tail...>>>;
};

template <class Keys> struct min_;
template <int Single> struct min_<keyset<Single>> {
	static constexpr int result = Single;
};
template <int Head, int... Tail> 
struct min_<keyset<Head, Tail...>> {
	static constexpr int tail = min_<keyset<Tail...>>::result; 
	static constexpr int result = Head < tail ? Head : tail;
};
template <class Keys> static constexpr int min = min_<Keys>::result;

template <class Keys> struct sort_;
template <class Keys> using sort = typename sort_<Keys>::result;

template <> 
struct sort_<keyset<>> {
	using result = keyset<>;
};
template <int... Keys> 
struct sort_<keyset<Keys...>> {
	static constexpr int first = min<keyset<Keys...>>;
	using result = prepend<first, sort<remove<first, keyset<Keys...>>>>;
};

}}