#include "wocca/magic/tuple.h"
#include "test/magic.h"
using namespace wocca::magic::tuple;

TEST_CASE("wocca/magic/tuple") {
    CHECK_META(std::tuple<>, empty);

    struct t1 {};
    struct t2 {};
    struct t3 {};
    struct t4 {};

    typedef std::tuple<t1, t2, t3> tuple;
    CHECK_META(tuple, make<t1, t2, t3>);

    CHECK_FALSE((contains<t1, empty>::value));
    CHECK((contains<t1, tuple>::value));
    CHECK((contains<t2, tuple>::value));
    CHECK((contains<t3, tuple>::value));
    CHECK_FALSE((contains<t4, tuple>::value));

    CHECK_FALSE((before<t1,t1,tuple>::value));
    CHECK      ((before<t1,t2,tuple>::value));
    CHECK      ((before<t1,t3,tuple>::value));
    CHECK_FALSE((before<t1,t4,tuple>::value));
    CHECK_FALSE((before<t2,t1,tuple>::value));
    CHECK_FALSE((before<t2,t2,tuple>::value));
    CHECK      ((before<t2,t3,tuple>::value));
    CHECK_FALSE((before<t2,t4,tuple>::value));
    CHECK_FALSE((before<t3,t1,tuple>::value));
    CHECK_FALSE((before<t3,t2,tuple>::value));
    CHECK_FALSE((before<t3,t3,tuple>::value));
    CHECK_FALSE((before<t3,t4,tuple>::value));
    CHECK_FALSE((before<t4,t1,tuple>::value));
    CHECK_FALSE((before<t4,t2,tuple>::value));
    CHECK_FALSE((before<t4,t3,tuple>::value));
    CHECK_FALSE((before<t4,t4,tuple>::value));

    typedef std::tuple<t1, t1, t2, t3> p1;
    typedef std::tuple<t2, t1, t2, t3> p2;
    typedef std::tuple<t3, t1, t2, t3> p3;
    typedef std::tuple<t4, t1, t2, t3> p4;
    CHECK_META(p1, prepend<t1, tuple>);
    CHECK_META(p2, prepend<t2, tuple>);
    CHECK_META(p3, prepend<t3, tuple>);
    CHECK_META(p4, prepend<t4, tuple>);

    CHECK_META(tuple, prepend<t1, tuple, false>);
    CHECK_META(tuple, prepend<t2, tuple, false>);
    CHECK_META(tuple, prepend<t3, tuple, false>);
    CHECK_META(tuple, prepend<t4, tuple, false>);

    CHECK_META(tuple, prepend_unique<t1, tuple>);
    CHECK_META(tuple, prepend_unique<t2, tuple>);
    CHECK_META(tuple, prepend_unique<t3, tuple>);
    CHECK_META(p4,    prepend_unique<t4, tuple>);

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
