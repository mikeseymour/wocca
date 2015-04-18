#include "wocca/magic/graph.h"

using namespace wocca::magic;

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

typedef std::tuple<
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

typedef graph::nodes<edges>::result nodes;
static_assert(tuple::size<nodes>() == 12, "");
static_assert(tuple::contains<A, nodes>(), "");
static_assert(tuple::contains<B, nodes>(), "");
static_assert(tuple::contains<C, nodes>(), "");
static_assert(tuple::contains<D, nodes>(), "");
static_assert(tuple::contains<E, nodes>(), "");
static_assert(tuple::contains<F, nodes>(), "");
static_assert(tuple::contains<G, nodes>(), "");
static_assert(tuple::contains<H, nodes>(), "");
static_assert(tuple::contains<I, nodes>(), "");
static_assert(tuple::contains<J, nodes>(), "");
static_assert(tuple::contains<K, nodes>(), "");
static_assert(tuple::contains<L, nodes>(), "");

typedef graph::adj<A, edges>::result adjA;
static_assert(tuple::size<adjA>() == 2, "");
static_assert(tuple::contains<B, adjA>(), "");
static_assert(tuple::contains<C, adjA>(), "");

typedef graph::adj<B, edges>::result adjB;
static_assert(tuple::size<adjB>() == 1, "");
static_assert(tuple::contains<D, adjB>(), "");

typedef graph::adj<C, edges>::result adjC;
static_assert(tuple::size<adjC>() == 1, "");
static_assert(tuple::contains<F, adjC>(), "");

typedef graph::adj<D, edges>::result adjD;
static_assert(tuple::size<adjD>() == 1, "");
static_assert(tuple::contains<E, adjD>(), "");

typedef graph::adj<E, edges>::result adjE;
static_assert(tuple::size<adjE>() == 1, "");
static_assert(tuple::contains<G, adjE>(), "");

typedef graph::adj<F, edges>::result adjF;
static_assert(tuple::size<adjF>() == 1, "");
static_assert(tuple::contains<G, adjF>(), "");

typedef graph::adj<G, edges>::result adjG;
static_assert(tuple::size<adjG>() == 2, "");
static_assert(tuple::contains<J, adjG>(), "");
static_assert(tuple::contains<K, adjG>(), "");

typedef graph::adj<H, edges>::result adjH;
static_assert(tuple::size<adjH>() == 1, "");
static_assert(tuple::contains<K, adjH>(), "");

typedef graph::adj<I, edges>::result adjI;
static_assert(tuple::size<adjI>() == 1, "");
static_assert(tuple::contains<L, adjI>(), "");

typedef graph::adj<J, edges>::result adjJ;
static_assert(tuple::size<adjJ>() == 0, "");

typedef graph::adj<K, edges>::result adjK;
static_assert(tuple::size<adjK>() == 0, "");

typedef graph::adj<L, edges>::result adjL;
static_assert(tuple::size<adjL>() == 0, "");

typedef graph::sort<edges>::result sorted;
static_assert(tuple::size<sorted>() == 12, "");
static_assert(tuple::contains<A, sorted>(), "");
static_assert(tuple::contains<B, sorted>(), "");
static_assert(tuple::contains<C, sorted>(), "");
static_assert(tuple::contains<D, sorted>(), "");
static_assert(tuple::contains<E, sorted>(), "");
static_assert(tuple::contains<F, sorted>(), "");
static_assert(tuple::contains<G, sorted>(), "");
static_assert(tuple::contains<H, sorted>(), "");
static_assert(tuple::contains<I, sorted>(), "");
static_assert(tuple::contains<J, sorted>(), "");
static_assert(tuple::contains<K, sorted>(), "");
static_assert(tuple::contains<L, sorted>(), "");

static_assert(tuple::before<A, B, sorted>(), "");
static_assert(tuple::before<A, C, sorted>(), "");
static_assert(tuple::before<B, D, sorted>(), "");
static_assert(tuple::before<C, F, sorted>(), "");
static_assert(tuple::before<D, E, sorted>(), "");
static_assert(tuple::before<E, G, sorted>(), "");
static_assert(tuple::before<F, G, sorted>(), "");
static_assert(tuple::before<G, J, sorted>(), "");
static_assert(tuple::before<G, K, sorted>(), "");
static_assert(tuple::before<H, K, sorted>(), "");
static_assert(tuple::before<I, L, sorted>(), "");
