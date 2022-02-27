/* BFS
 * USAGE
 *  dijkstra bfs(graph, source, inf);
 * OUTPUT
 *  distance from source `get_dists()`, bfs tree `get_parents()`
 *  dist = inf means unreachable
 * TIME
 *  O(V + ElogE) maybe
 *  V = #vertices, E = #edges
 * STATUS
 *  untested: cf/20c
 */
#pragma once

template <typename graph_t>
struct dijkstra {
  using weight_t = get_graph_weight_t<graph_t>;
  vector<weight_t> dist;
  vector<int> parent;
  dijkstra(const graph_t& graph, int source, weight_t inf):
    dist(graph.size(), inf), parent(graph.size(), -1) {
    priority_queue<Item> to_visit;
    to_visit.emplace(source, weight_t());
    dist[source] = weight_t();
    // parent[source] = source;
    while (!to_visit.empty()) {
      auto [u, d] = to_visit.top();
      to_visit.pop();
      if (d != dist[u]) continue;
      for (const auto& [v, c] : graph[u]) {
        if (dist[v] != inf && dist[v] <= d + c) continue;
        dist[v] = d + c;
        parent[v] = u;
        to_visit.emplace(v, dist[v]);
      }
    }
  }
  const vector<weight_t>& get_dists() const { return dist; }
  const vector<int>& get_parents() const { return parent; }
  vector<int> get_path(int to) const {
    vector<int> path;
    for ( ; to != -1; to = parent[to]) {
      path.push_back(to);
    }
    reverse(path.begin(), path.end());
    return path;
  }
  struct Item {
    int u;
    weight_t d;
    bool operator < (const Item& o) const { return d > o.d; }
  };
};

