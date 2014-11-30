#ifndef WOCCA_MAGIC_GRAPH_H
#define WOCCA_MAGIC_GRAPH_H

#include "wocca/magic/tuple.h"

namespace wocca {
namespace magic {
namespace graph {

// An edge of a directed graph. The direction is from Head to Tail.
template <typename Head, typename Tail> struct edge {};

// Unordered set of nodes from a set of edges
template <typename... Edges> struct nodes;
template <> struct nodes<> : tuple::empty {};
template <typename Head, typename Tail, typename... Edges> struct nodes<edge<Head, Tail>, Edges...> :
    tuple::prepend_unique<Head, tuple::prepend_unique<Tail, nodes<Edges...>>> {};

// The set of nodes adjacent to this one; i.e. those at the tails of edges for which this is the head.
template <typename Node, typename... Edges> struct adj;
template <typename Node> struct adj<Node> : tuple::empty {};
template <typename Node, typename Edge, typename... Edges> struct adj<Node, Edge, Edges...> : adj<Node, Edges...> {};
template <typename Head, typename Tail, typename... Edges> struct adj<Head, edge<Head, Tail>, Edges...> :
    tuple::prepend_unique<Tail, adj<Head, Edges...>> {};

// Depth-first topological sort details
namespace dfs_sort {
    template <typename Nodes, typename Visiting, typename Visited, typename... Edges> struct visit :
        visit<self<Nodes>, self<Visiting>, self<Visited>, Edges...> {};

    template <bool already_visited, typename Node, typename Visiting, typename Visited, typename... Edges> struct visit_adj;
    template <typename Node, typename Visiting, typename Visited, typename... Edges>
    struct visit_adj<true, Node, Visiting, Visited, Edges...> : self<Visited> {};
    template <typename Node, typename Visiting, typename Visited, typename... Edges>
    struct visit_adj<false, Node, Visiting, Visited, Edges...> :
        tuple::prepend<Node, visit<adj<Node, Edges...>, tuple::prepend<Node, Visiting>, Visited, Edges...>> {};

    template <typename Visited, typename Visiting, typename... Edges> struct visit<tuple::empty, Visiting, Visited, Edges...> : self<Visited> {};
    template <typename Node, typename... Rest, typename Visiting, typename Visited, typename... Edges>
    struct visit<tuple::make<Node, Rest...>, Visiting, Visited, Edges...> :
        visit<
            tuple::make<Rest...>,
            Visiting,
            visit_adj<tuple::contains<Node, Visiting>::value || tuple::contains<Node, Visited>::value, Node, Visiting, Visited, Edges...>,
            Edges...
        >
    {
        static_assert(!tuple::contains<Node, Visiting>::value, "Cycle detected");
    };
}

// Topologically sort the graph, giving the set of nodes ordered so that the
// head of each edge comes before the corresponding tail.
template <typename... Edges> struct sort :
    dfs_sort::visit<nodes<Edges...>, tuple::empty, tuple::empty, Edges...> {};

}}}
#endif
