/* Eulerian Trail
 * USAGE
 *  vector<int> euler_trail<vertices?>(graph, num_edges, source);
 * INPUT
 *  graph: vector<vector<pair<int, int>>> an adjacency list of [neighbour, edge_id].
 *    edge_id can be -1 for directed graphs
 *  num_edges: number of edges. should satisfy edge_id < num_edges for all edges.
 *  source: the vertex at which the trail should start
 *  vertices?: bool specifying whether to return the trail as vertices or edges,
 *    default is true.
 * RETURN
 *  returns the eulerian trail, either E+1 vertices, or E edge ids
 *  returns an empty vector if no trail found
 * TIME
 *  O(E)
 *  E = #edges
 * STATUS
 *  tested: kattis/eulerianpath
 *  (undirected and edges are untested)
 */
#pragma once

template <bool vertices = true>
vector<int> euler_trail(const vector<vector<pair<int, int>>>& graph, int num_edges, int source) {
  vector<bool> used(num_edges);
  vector<int> diff(size(graph)), cur_edge(size(graph)), trail;
  vector<conditional_t<vertices, int, pair<int, int>>> stk;
  stk.reserve(size(graph));
  diff[source]++;
  if constexpr (vertices) stk.push_back(source);
  else stk.emplace_back(source, -1);
  while (!empty(stk)) {
    int u;
    if constexpr (vertices) u = stk.back();
    else u = stk.back().first;
    if (cur_edge[u] == (int)size(graph[u])) {
      if constexpr (vertices) trail.push_back(u);
      else trail.push_back(stk.back().second);
      stk.pop_back();
    } else {
      auto [v, e] = graph[u][cur_edge[u]++];
      if (e == -1 || !used[e]) {
        if (e != -1) used[e] = true;
        diff[u]--;
        diff[v]++;
        if constexpr (vertices) stk.push_back(v);
        else stk.emplace_back(v, e);
      }
    }
  }
  assert((int)size(trail) <= num_edges + vertices);
  if ((int)size(trail) != num_edges + vertices
      || *min_element(begin(diff), end(diff)) < 0) {
    return {};
  }
  return vector(rbegin(trail), rend(trail));
}

