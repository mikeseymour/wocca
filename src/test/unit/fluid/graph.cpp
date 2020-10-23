#include "wocca/fluid/graph.h"
#include "test/catch.hpp"

#include <algorithm>

using namespace wocca::fluid::graph;

template <class Range, class Thing>
static bool contains(Range&& r, Thing&& t)
{
  return std::find(r.begin(), r.end(), t) != r.end();
}

template <class Range, class Thing>
static bool before(Range&& r, Thing&& t1, Thing&& t2)
{
  return std::find(std::find(r.begin(), r.end(), t1), r.end(), t2) != r.end();
}

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

static struct node {} nodes[12];
static node* A = &nodes[0];
static node* B = &nodes[1];
static node* C = &nodes[2];
static node* D = &nodes[3];
static node* E = &nodes[4];
static node* F = &nodes[5];
static node* G = &nodes[6];
static node* H = &nodes[7];
static node* I = &nodes[8];
static node* J = &nodes[9];
static node* K = &nodes[10];
static node* L = &nodes[11];

static const std::vector<edge<node>> edges {
  {C,F}, {D,E}, {B,D}, {A,B}, {A,C}, {G,J}, {I,L}, {F,G}, {G,K}, {E,G}, {H,K}
};

TEST_CASE("wocca/fluid/graph/adjacency/forward", "Graph adjacency (forward)"){
  auto adj = adjacency(edges);
  CHECK(adj.size() == 12);

  CHECK(adj.at(A).size() == 2);
  CHECK(contains(adj.at(A), B));
  CHECK(contains(adj.at(A), C));

  CHECK(adj.at(B).size() == 1);
  CHECK(contains(adj.at(B), D));

  CHECK(adj.at(C).size() == 1);
  CHECK(contains(adj.at(C), F));

  CHECK(adj.at(D).size() == 1);
  CHECK(contains(adj.at(D), E));

  CHECK(adj.at(E).size() == 1);
  CHECK(contains(adj.at(E), G));

  CHECK(adj.at(F).size() == 1);
  CHECK(contains(adj.at(F), G));

  CHECK(adj.at(G).size() == 2);
  CHECK(contains(adj.at(G), J));
  CHECK(contains(adj.at(G), K));

  CHECK(adj.at(H).size() == 1);
  CHECK(contains(adj.at(H), K));

  CHECK(adj.at(I).size() == 1);
  CHECK(contains(adj.at(I), L));

  CHECK(adj.at(J).size() == 0);
  CHECK(adj.at(K).size() == 0);
  CHECK(adj.at(L).size() == 0);
}

TEST_CASE("wocca/fluid/graph/adjacency/reverse", "Graph adjacency (reverse)"){
  auto adj = adjacency(edges, direction::reverse);
  CHECK(adj.size() == 12);

  CHECK(adj.at(A).size() == 0);

  CHECK(adj.at(B).size() == 1);
  CHECK(contains(adj.at(B), A));

  CHECK(adj.at(C).size() == 1);
  CHECK(contains(adj.at(C), A));

  CHECK(adj.at(D).size() == 1);
  CHECK(contains(adj.at(D), B));

  CHECK(adj.at(E).size() == 1);
  CHECK(contains(adj.at(E), D));

  CHECK(adj.at(F).size() == 1);
  CHECK(contains(adj.at(F), C));

  CHECK(adj.at(G).size() == 2);
  CHECK(contains(adj.at(G), E));
  CHECK(contains(adj.at(G), F));

  CHECK(adj.at(H).size() == 0);
  CHECK(adj.at(I).size() == 0);

  CHECK(adj.at(J).size() == 1);
  CHECK(contains(adj.at(J), G));

  CHECK(adj.at(K).size() == 2);
  CHECK(contains(adj.at(K), G));
  CHECK(contains(adj.at(K), H));

  CHECK(adj.at(L).size() == 1);
  CHECK(contains(adj.at(L), I));
}

TEST_CASE("wocca/fluid/graph/sort/forward", "Graph sort (forward)"){
  auto sorted = sort(edges);

  CHECK(sorted.size() == 12);
  CHECK(contains(sorted, A));
  CHECK(contains(sorted, B));
  CHECK(contains(sorted, C));
  CHECK(contains(sorted, D));
  CHECK(contains(sorted, E));
  CHECK(contains(sorted, F));
  CHECK(contains(sorted, G));
  CHECK(contains(sorted, H));
  CHECK(contains(sorted, I));
  CHECK(contains(sorted, J));
  CHECK(contains(sorted, K));
  CHECK(contains(sorted, L));

  CHECK(before(sorted, A, B));
  CHECK(before(sorted, A, C));
  CHECK(before(sorted, B, D));
  CHECK(before(sorted, C, F));
  CHECK(before(sorted, D, E));
  CHECK(before(sorted, E, G));
  CHECK(before(sorted, F, G));
  CHECK(before(sorted, G, J));
  CHECK(before(sorted, G, K));
  CHECK(before(sorted, H, K));
  CHECK(before(sorted, I, L));
}

TEST_CASE("wocca/fluid/graph/sort/backward", "Graph sort (reverse)"){
  auto sorted = sort(edges, direction::reverse);

  CHECK(sorted.size() == 12);
  CHECK(contains(sorted, A));
  CHECK(contains(sorted, B));
  CHECK(contains(sorted, C));
  CHECK(contains(sorted, D));
  CHECK(contains(sorted, E));
  CHECK(contains(sorted, F));
  CHECK(contains(sorted, G));
  CHECK(contains(sorted, H));
  CHECK(contains(sorted, I));
  CHECK(contains(sorted, J));
  CHECK(contains(sorted, K));
  CHECK(contains(sorted, L));

  CHECK(before(sorted, B, A));
  CHECK(before(sorted, C, A));
  CHECK(before(sorted, D, B));
  CHECK(before(sorted, E, D));
  CHECK(before(sorted, F, C));
  CHECK(before(sorted, G, E));
  CHECK(before(sorted, G, F));
  CHECK(before(sorted, J, G));
  CHECK(before(sorted, K, G));
  CHECK(before(sorted, K, H));
  CHECK(before(sorted, L, I));
}

TEST_CASE("wocca/fluid/graph/sort/cyclic", "Graph sort (cyclic)"){
  const std::vector<edge<node>> cycle { {A,B}, {B,C}, {C,A} };
  CHECK_THROWS_AS(sort(cycle), cyclic);
}
