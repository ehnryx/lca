/* Graph (???)
 * USAGE
 *  graph_list<edge_t> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, weight);
 * STATUS
 *  untested
 */
#pragma once

#include <vector>

struct graph_traversal {
  std::vector<int> parent;
  graph_traversal(int n): parent(n, -1) {}
  std::vector<int> get_path(int to) const {
    std::vector<int> path;
    if (parent[to] != -1) {
      while (true) {
        path.push_back(to);
        if (to == parent[to]) break;
        to = parent[to];
      }
      reverse(path.begin(), path.end());
    }
    return path;
  }
  const std::vector<int>& get_parents() const { return parent; }
  bool visited(int u) const { return parent[u] != -1; }

  struct callback_placeholder_t {};
  template <typename node_func_t = callback_placeholder_t,
            typename edge_func_t = callback_placeholder_t,
            typename should_visit_t = callback_placeholder_t>
  struct callbacks_t {
    node_func_t const& node_func = {};
    edge_func_t const& edge_func = {};
    should_visit_t const& should_visit = {};
  };
#define IS_CALLBACK_PLACEHOLDER(var) \
  std::is_same_v<std::decay_t<decltype(var)>, callback_placeholder_t>
};

