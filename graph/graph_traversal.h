/* Graph (???)
 * USAGE
 *  graph_list<edge_t> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, weight);
 * STATUS
 *  untested
 */
#pragma once

struct graph_traversal {
  virtual const vector<int>& get_parents() const = 0;
  vector<int> get_path(int to) const {
    vector<int> path;
    for ( ; to != -1; to = get_parents()[to]) {
      path.push_back(to);
    }
    reverse(path.begin(), path.end());
    return path;
  }
};
