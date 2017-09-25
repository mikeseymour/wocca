#include "wocca/magic/tuple.h"
#include "test/catch.hpp"

namespace {
using namespace wocca::magic;
using namespace tuples;

struct t1 {};
struct t2 {};
struct t3 {};
struct t4 {};

typedef tuple<> empty;
static_assert(size<empty> == 0);

typedef tuple<t1, t2, t3> three;
static_assert(size<three> == 3);

static_assert(!contains<t1,empty>);
static_assert( contains<t1,three>);
static_assert( contains<t2,three>);
static_assert( contains<t3,three>);
static_assert(!contains<t4,three>);

static_assert(!before<t1,t1,empty>);
static_assert(!before<t1,t2,empty>);
static_assert(!before<t1,t1,three>);
static_assert( before<t1,t2,three>);
static_assert( before<t1,t3,three>);
static_assert(!before<t1,t4,three>);
static_assert(!before<t2,t1,three>);
static_assert(!before<t2,t2,three>);
static_assert( before<t2,t3,three>);
static_assert(!before<t2,t4,three>);
static_assert(!before<t3,t1,three>);
static_assert(!before<t3,t2,three>);
static_assert(!before<t3,t3,three>);
static_assert(!before<t3,t4,three>);
static_assert(!before<t4,t1,three>);
static_assert(!before<t4,t2,three>);
static_assert(!before<t4,t3,three>);
static_assert(!before<t4,t4,three>);

static_assert(same<prepend<t1,tuple<>>, tuple<t1>>);
static_assert(same<prepend<t1,tuple<t2>>, tuple<t1,t2>>);
static_assert(same<prepend<t1,tuple<t2,t3>>, tuple<t1,t2,t3>>);
static_assert(same<prepend<t1,tuple<t2,t3,t4>>, tuple<t1, t2, t3, t4>>);

TEST_CASE("wocca/magic/tuple/visit")
{
    std::ostringstream check;
    auto f = [&](auto const & x){check << x;};
    wocca::magic::tuple().visit(f);
    CHECK(check.str() == "");
    wocca::magic::tuple('a','b','c',1,2,3).visit(f);
    CHECK(check.str() == "abc123");
    wocca::magic::tuple{'d','e','f',4,5,6}.rvisit(f);
    CHECK(check.str() == "abc123654fed");
}

}
