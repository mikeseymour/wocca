#include "wocca/tags/keyset.h"

namespace t = wocca::tags;
using namespace wocca::tags;

template <class, class> constexpr bool same = false;
template <class T> constexpr bool same<T,T> = true;

static_assert(keyset<1,2,3>::keys[1] == 2);

static_assert(same<prepend<1,keyset<>>, keyset<1>>);
static_assert(same<prepend<1,keyset<2,3>>, keyset<1,2,3>>);

static_assert(same<t::remove<1,keyset<2,3>>, keyset<2,3>>);
static_assert(same<t::remove<1,keyset<1,2,3>>, keyset<2,3>>);
static_assert(same<t::remove<1,keyset<2,1,3>>, keyset<2,3>>);
static_assert(same<t::remove<1,keyset<2,3,1>>, keyset<2,3>>);
static_assert(same<t::remove<1,keyset<1,1,2,1,3,1>>, keyset<1,2,1,3,1>>);

static_assert(min<keyset<5,4,3,2,1>> == 1);
static_assert(min<keyset<1,5,4,3,2>> == 1);
static_assert(min<keyset<5,4,1,3,2>> == 1);

static_assert(same<sort<keyset<5,3,4,3,2,1>>, keyset<1,2,3,3,4,5>>);