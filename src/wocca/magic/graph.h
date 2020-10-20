#pragma once
#include "wocca/magic/tuple.h"

namespace wocca {
namespace magic {
namespace graph {

using namespace tuples;

// An edge of a directed graph. The direction is from Head to Tail.
template <class Head, class Tail> struct edge {};

// Prepend a node to a tuple if it's not already there
template <class Node, class Tuple> using prepend_unique_ =
    conditional_<contains<Node,Tuple>, Tuple, prepend<Node,Tuple>>;
template <class Node, class Tuple> using prepend_unique =
    unwrap<prepend_unique_<Node, Tuple>>;

// Unordered set of nodes derived from a set of edges
template <class Edges> struct nodes_ : wrap<tuple<>> {};
template <class Edges> using nodes = unwrap<nodes_<Edges>>;

template <class Head, class Tail, class... Edges> 
struct nodes_<tuple<edge<Head,Tail>, Edges...>> :
    prepend_unique_<Head, prepend_unique<Tail, nodes<tuple<Edges...>>>> {};

// The set of nodes adjacent to this one; i.e. those at the tails of edges for
// which this is the head.
template <class Node, class Edges> struct adj_ : wrap<tuple<>> {};
template <class Node, class Edges> using adjacent = unwrap<adj_<Node,Edges>>;

template <class Node, class Head, class Tail, class... Edges> 
struct adj_<Node, tuple<edge<Head,Tail>, Edges...>> {
    using rest = adjacent<Node, tuple<Edges...>>;
    using type = conditional<same<Node,Head>, prepend_unique<Tail,rest>, rest>;
};

// Topologically sort the graph, giving the set of nodes ordered so that the
// head of each edge comes before the corresponding tail.
template <class Sorted_, class Visiting, class Edges, class Nodes>
    struct sort_ : Sorted_ {};

struct cyclic : wrap<cyclic> {};

template <class Node, class Sorted>
    struct prepend_node_ : prepend_<Node, Sorted> {};
template <class Node>
    struct prepend_node_<Node, cyclic> : cyclic {};

template <class Sorted_, class Visiting, class Edges, class Node, class Nodes>
struct sort_no_cycle_ :
    sort_<
        conditional<
            contains<Node, unwrap<Sorted_>>,
            Sorted_,
            prepend_node_<
                Node,
                unwrap<sort_<
                    Sorted_,
                    prepend<Node, Visiting>,
                    Edges,
                    adjacent<Node, Edges>
                >>
            >
        >,
        Visiting, Edges, Nodes
    > {};

template <class Sorted_, class Visiting, class Edges, class Node, class... Nodes>
struct sort_<Sorted_, Visiting, Edges, tuple<Node, Nodes...>> :
    conditional<
        same<Sorted_, cyclic> || contains<Node, Visiting>,
        cyclic,
        sort_no_cycle_<Sorted_, Visiting, Edges, Node, tuple<Nodes...>>
    > {};

template <class Edges>
using sort = unwrap<sort_<wrap<tuple<>>, tuple<>, Edges, nodes<Edges>>>;

}}}

