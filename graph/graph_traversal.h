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
  virtual ~graph_traversal() = default;
  virtual const std::vector<int>& get_parents() const = 0;
  std::vector<int> get_path(int to) const {
    std::vector<int> path;
    if (get_parents()[to] != -1) {
      while (true) {
        path.push_back(to);
        if (to == get_parents()[to]) break;
        to = get_parents()[to];
      }
      reverse(path.begin(), path.end());
    }
    return path;
  }
};
