#include "wocca/magic/graph.h"

namespace {
using namespace wocca::magic;
using namespace tuples;

// An acyclic graph with splits, joins, and disjoint sections:
//
//                     A
//                    / \          (not a multiline comment)
//                   B   C
//                   |   |
//                   D   |
//                   |   |
//                   E   F
//                    \ /
//                     G   H  I
//                    / \ /   |
//                   J   K    L
struct A {};
struct B {};
struct C {};
struct D {};
struct E {};
struct F {};
struct G {};
struct H {};
struct I {};
struct J {};
struct K {};
struct L {};

typedef tuple<
    graph::edge<C,F>,
    graph::edge<D,E>,
    graph::edge<B,D>,
    graph::edge<A,B>,
    graph::edge<A,C>,
    graph::edge<G,J>,
    graph::edge<I,L>,
    graph::edge<F,G>,
    graph::edge<G,K>,
    graph::edge<E,G>,
    graph::edge<H,K>
> edges;

using nodes = graph::nodes<edges>;
static_assert(size<nodes> == 12);
static_assert(contains<A, nodes>);
static_assert(contains<B, nodes>);
static_assert(contains<C, nodes>);
static_assert(contains<D, nodes>);
static_assert(contains<E, nodes>);
static_assert(contains<F, nodes>);
static_assert(contains<G, nodes>);
static_assert(contains<H, nodes>);
static_assert(contains<I, nodes>);
static_assert(contains<J, nodes>);
static_assert(contains<K, nodes>);
static_assert(contains<L, nodes>);

typedef graph::adjacent<A, edges> adjA;
static_assert(size<adjA> == 2);
static_assert(contains<B, adjA>);
static_assert(contains<C, adjA>);

typedef graph::adjacent<B, edges> adjB;
static_assert(size<adjB> == 1);
static_assert(contains<D, adjB>);

typedef graph::adjacent<C, edges> adjC;
static_assert(size<adjC> == 1);
static_assert(contains<F, adjC>);

typedef graph::adjacent<D, edges> adjD;
static_assert(size<adjD> == 1);
static_assert(contains<E, adjD>);

typedef graph::adjacent<E, edges> adjE;
static_assert(size<adjE> == 1);
static_assert(contains<G, adjE>);

typedef graph::adjacent<F, edges> adjF;
static_assert(size<adjF> == 1);
static_assert(contains<G, adjF>);

typedef graph::adjacent<G, edges> adjG;
static_assert(size<adjG> == 2);
static_assert(contains<J, adjG>);
static_assert(contains<K, adjG>);

typedef graph::adjacent<H, edges> adjH;
static_assert(size<adjH> == 1);
static_assert(contains<K, adjH>);

typedef graph::adjacent<I, edges> adjI;
static_assert(size<adjI> == 1);
static_assert(contains<L, adjI>);

typedef graph::adjacent<J, edges> adjJ;
static_assert(size<adjJ> == 0);

typedef graph::adjacent<K, edges> adjK;
static_assert(size<adjK> == 0);

typedef graph::adjacent<L, edges> adjL;
static_assert(size<adjL> == 0);

typedef graph::sort<edges> sorted;
static_assert(size<sorted> == 12);
static_assert(contains<A, sorted>);
static_assert(contains<B, sorted>);
static_assert(contains<C, sorted>);
static_assert(contains<D, sorted>);
static_assert(contains<E, sorted>);
static_assert(contains<F, sorted>);
static_assert(contains<G, sorted>);
static_assert(contains<H, sorted>);
static_assert(contains<I, sorted>);
static_assert(contains<J, sorted>);
static_assert(contains<K, sorted>);
static_assert(contains<L, sorted>);

static_assert(before<A, B, sorted>);
static_assert(before<A, C, sorted>);
static_assert(before<B, D, sorted>);
static_assert(before<C, F, sorted>);
static_assert(before<D, E, sorted>);
static_assert(before<E, G, sorted>);
static_assert(before<F, G, sorted>);
static_assert(before<G, J, sorted>);
static_assert(before<G, K, sorted>);
static_assert(before<H, K, sorted>);
static_assert(before<I, L, sorted>);

#if 0
// TODO report cycle by setting a flag rather than asserting
typedef tuple<
    graph::edge<A,B>,
    graph::edge<B,C>,
    graph::edge<C,A>
> cycle;
typedef graph::sort<cycle> boom;
#endif

}

