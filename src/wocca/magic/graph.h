#pragma once
#include "wocca/magic/tuple.h"

namespace wocca {
namespace magic {
namespace graph {

using namespace tuples;

// An edge of a directed graph. The direction is from Head to Tail.
template <class Head, class Tail> struct edge {};

// Prepend a node to a tuple if it's not already there
template <class Node, class Tuple> 
using prepend_unique = conditional<contains<Node,Tuple>, Tuple, prepend<Node,Tuple>>;

// Unordered set of nodes derived from a set of edges
template <class Edges> struct nodes_ {using type = tuple<>;};
template <class Edges> using nodes = typename nodes_<Edges>::type;

template <class Head, class Tail, class... Edges> 
struct nodes_<tuple<edge<Head,Tail>, Edges...>> {
    using type = prepend_unique<Head, prepend_unique<Tail, nodes<tuple<Edges...>>>>;
};

// The set of nodes adjacent to this one; i.e. those at the tails of edges for which this is the head.
template <class Node, class Edges> struct adj_{using type = tuple<>;};
template <class Node, class Edges> using adj = typename adj_<Node,Edges>::type;

template <class Node, class Edge, class... Edges> 
struct adj_<Node, tuple<Edge, Edges...>> : adj_<Node, tuple<Edges...>> {};

template <class Head, class Tail, class... Edges> 
struct adj_<Head, tuple<edge<Head, Tail>, Edges...>> {
    using type = prepend_unique<Tail, adj<Head, tuple<Edges...>>>;
};

// Topologically sort the graph, giving the set of nodes ordered so that the
// head of each edge comes before the corresponding tail.
template <class Edges, class Nodes = nodes<Edges>, class Visited = tuple<>> 
    struct sort_ {using type = Visited;};

template <class Edges, class Visited, class Node, class... Nodes>
struct sort_<Edges, tuple<Node, Nodes...>, Visited> {
    using add_adj = typename sort_<Edges, adj<Node, Edges>, Visited>::type;
    using add_node = conditional<contains<Node, Visited>, Visited, prepend<Node, add_adj>>;
    using type = typename sort_<Edges, tuple<Nodes...>, add_node>::type;
};

// With cycle detection
template <class Edges, class Nodes = nodes<Edges>, class Visited = tuple<>, class Visiting = tuple<>> 
    struct sort_safe {using type = Visited;};

template <class Edges, class Visited, class Visiting, class Node, class... Nodes>
struct sort_safe<Edges, tuple<Node, Nodes...>, Visited, Visiting> {
    static_assert(!contains<Node, Visiting>, "Cycle detected");
    using add_adj = typename sort_safe<Edges, adj<Node, Edges>, Visited, prepend<Node, Visiting>>::type;
    using add_node = conditional<contains<Node, Visited>, Visited, prepend<Node, add_adj>>;
    using type = typename sort_safe<Edges, tuple<Nodes...>, add_node, Visiting>::type;
};

template <class Edges> using sort = typename sort_<Edges>::type;

}}}

