#pragma once
#include "wocca/tags/basic.h"
#include "wocca/tags/keyset.h"

#include <array>

namespace wocca {
namespace tags {

template <int... Keys> 
struct parser {
	template <typename Reader> parser(Reader && reader) {
		while (tag t = reader()) {
			int i = index(t.key);
			if (i >= 0) {
				values[i] = t.value;
			}
		}
	}

	template <int key> view at() {
		static_assert(index(key) >= 0);
		return values[index(key)];
	}

	static constexpr int index(int key) {
		using sorted = sort<keyset<Keys...>>;
		int first = 0, last = std::size(sorted::keys);
		while (first != last) {
			int mid = first + (last-first)/2;
			if (sorted::keys[mid] == key) return mid;
			if (sorted::keys[mid] < key) {
				first = mid+1;
			} else {
				last = mid;
			}
		}
		return -1;
	}

	std::array<view, sizeof...(Keys)> values;
};

}}