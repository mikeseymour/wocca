#include "wocca/magic/tuple.h"
#include "test/catch.hpp"

namespace {
using namespace wocca::magic::tuple;

static_assert(std::is_same<std::tuple<>, empty::result>(), "");

struct t1 {};
struct t2 {};
struct t3 {};
struct t4 {};

typedef std::tuple<t1, t2, t3> tuple;
static_assert(std::is_same<tuple, make<t1, t2, t3>::result>(), "");

static_assert(!contains<t1, empty>(), "");
static_assert( contains<t1, tuple>(), "");
static_assert( contains<t2, tuple>(), "");
static_assert( contains<t3, tuple>(), "");
static_assert(!contains<t4, tuple>(), "");

static_assert(!before<t1,t1,tuple>(), "");
static_assert( before<t1,t2,tuple>(), "");
static_assert( before<t1,t3,tuple>(), "");
static_assert(!before<t1,t4,tuple>(), "");
static_assert(!before<t2,t1,tuple>(), "");
static_assert(!before<t2,t2,tuple>(), "");
static_assert( before<t2,t3,tuple>(), "");
static_assert(!before<t2,t4,tuple>(), "");
static_assert(!before<t3,t1,tuple>(), "");
static_assert(!before<t3,t2,tuple>(), "");
static_assert(!before<t3,t3,tuple>(), "");
static_assert(!before<t3,t4,tuple>(), "");
static_assert(!before<t4,t1,tuple>(), "");
static_assert(!before<t4,t2,tuple>(), "");
static_assert(!before<t4,t3,tuple>(), "");
static_assert(!before<t4,t4,tuple>(), "");

typedef std::tuple<t1, t1, t2, t3> p1;
typedef std::tuple<t2, t1, t2, t3> p2;
typedef std::tuple<t3, t1, t2, t3> p3;
typedef std::tuple<t4, t1, t2, t3> p4;
static_assert(std::is_same<p1, prepend<t1, tuple>::result>(), "");
static_assert(std::is_same<p2, prepend<t2, tuple>::result>(), "");
static_assert(std::is_same<p3, prepend<t3, tuple>::result>(), "");
static_assert(std::is_same<p4, prepend<t4, tuple>::result>(), "");

static_assert(std::is_same<tuple, prepend<t1, tuple, false>::result>(), "");
static_assert(std::is_same<tuple, prepend<t2, tuple, false>::result>(), "");
static_assert(std::is_same<tuple, prepend<t3, tuple, false>::result>(), "");
static_assert(std::is_same<tuple, prepend<t4, tuple, false>::result>(), "");

static_assert(std::is_same<tuple, prepend_unique<t1, tuple>::result>(), "");
static_assert(std::is_same<tuple, prepend_unique<t2, tuple>::result>(), "");
static_assert(std::is_same<tuple, prepend_unique<t3, tuple>::result>(), "");
static_assert(std::is_same<p4,    prepend_unique<t4, tuple>::result>(), "");

TEST_CASE("wocca/magic/tuple/visit")
{
    std::string visit_check;
    visit(std::tuple<>(), [&](char c){visit_check += c;});
    CHECK(visit_check == "");
    visit(std::make_tuple('a','b','c','d'), [&](char c){visit_check += c;});
    CHECK(visit_check == "abcd");
    auto t = std::make_tuple('e','f','g','h');
    visit(t, [&](char c){visit_check += c;});
    CHECK(visit_check == "abcdefgh");
}

TEST_CASE("wocca/magic/tuple/rvisit")
{
    std::string visit_check;
    rvisit(std::tuple<>(), [&](char c){visit_check += c;});
    CHECK(visit_check == "");
    rvisit(std::make_tuple('a','b','c','d'), [&](char c){visit_check += c;});
    CHECK(visit_check == "dcba");
    auto t = std::make_tuple('e','f','g','h');
    rvisit(t, [&](char c){visit_check += c;});
    CHECK(visit_check == "dcbahgfe");
}

}
