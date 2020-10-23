#pragma once

#include <exception>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cassert>

namespace wocca {
namespace fluid {
namespace graph {

struct cyclic : std::invalid_argument
{
  cyclic() : std::invalid_argument("Invalid graph: cycle detected") {}
};

template <class Node>
struct edge {
  using node_type = Node;
  Node* head;
  Node* tail;
};

enum class direction {forward, reverse};
direction operator~(direction d)
{
  return d == direction::forward ? direction::reverse : direction::forward;
}

template <class Node>
using adjacency_map = std::unordered_map<Node*, std::vector<Node*>>;

template <class Node>
void add_edge(adjacency_map<Node>& adj, edge<Node> edge)
{
  adj[edge.head].push_back(edge.tail);
  adj[edge.tail];
}

template <class Edges, class Node = typename Edges::value_type::node_type>
adjacency_map<Node> adjacency(Edges const& edges, direction d = direction::forward)
{
  adjacency_map<Node> adj;
  for (edge<Node> e : edges)
    add_edge(adj, d == direction::forward ? e : edge<Node>{e.tail, e.head});
  return adj;
}

template <class Edges, class Node = typename Edges::value_type::node_type>
std::vector<Node*> sort(Edges const& edges, direction d = direction::forward)
{
  auto adj = adjacency(edges, ~d);
  std::vector<Node*> sorted;
  std::vector<typename decltype(adj)::iterator> visiting;
  sorted.reserve(adj.size());
  visiting.reserve(adj.size());

  auto partial = [&](auto adj_it, auto&& recurse) -> void {
    auto const & tails = adj_it->second;

    if (!tails.empty()) {
      if (std::find(visiting.begin(), visiting.end(), adj_it) != visiting.end())
        throw cyclic{};

      visiting.push_back(adj_it);

      for (auto tail : tails) {
        auto tail_it = adj.find(tail);
        if (tail_it != adj.end())
          recurse(adj.find(tail), recurse);
      }

      visiting.pop_back();
    }

    sorted.push_back(adj_it->first);
    adj.erase(adj_it);
  };

  while (!adj.empty())
    partial(adj.begin(), partial);

  return sorted;
}

}}}
