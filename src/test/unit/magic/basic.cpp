#include "wocca/magic/basic.h"

namespace {
using namespace wocca::magic;

struct t1;
struct t2;
struct t3;
struct tX;

static_assert( same<t1,t1>);
static_assert(!same<t1,t2>);

static_assert( same<t1, conditional<true,  t1, t2>>);
static_assert(!same<t2, conditional<true,  t1, t2>>);
static_assert(!same<t1, conditional<false, t1, t2>>);
static_assert( same<t2, conditional<false, t1, t2>>);

}
