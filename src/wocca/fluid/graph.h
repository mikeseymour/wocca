#pragma once

#include <stdexcept>
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
  sorted.reserve(adj.size());

  auto adj_it = adj.begin();
  std::vector<decltype(adj_it)> visiting;

  while (adj_it != adj.end()) {
    if (std::find(visiting.begin(), visiting.end(), adj_it) != visiting.end())
      throw cyclic{};

    auto tail = adj.end();
    auto& tails = adj_it->second;
    while (!tails.empty() && tail == adj.end()) {
      tail = adj.find(tails.back());
      tails.pop_back();
    }

    if (tail == adj.end()) {
      sorted.push_back(adj_it->first);
      adj.erase(adj_it);
      if (visiting.empty()) {
        adj_it = adj.begin();
      } else {
        adj_it = visiting.back();
        visiting.pop_back();
      }
    } else {
      visiting.push_back(adj_it);
      adj_it = tail;
    }
  }

  return sorted;
}

}}}
