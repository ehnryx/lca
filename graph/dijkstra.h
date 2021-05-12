/* Dijkstra
 * USAGE
 *  auto dist = dijkstra(graph, source);
 * INPUT
 *  graph:  adjacency list of a non-negatively weighted directed graph
 *    graph[i] is a vector of pairs (v, cost of edge i -> v)
 *  source: the vertex from which to run Dijkstra
 * OUTPUT
 *  dist[i] = distance from source to i
 *  dist[i] = -1 if it is impossible to reach i from source
 * TIME
 *  O(V + ElogV)
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: kattis/shortestpath1
 */
#pragma once

template <typename T, class G>
vector<T> dijkstra(const G& graph, int source) {
  vector<T> dist(graph.size(), -1);
  struct Node {
    int to;
    T cost;
    Node(int a, const T& b): to(a), cost(b) {}
    bool operator < (const Node& o) const {
      return o.cost < this->cost;
    }
  };
  priority_queue<Node> todo;
  todo.emplace(source, 0);
  dist[source] = 0;
  vector<bool> vis(graph.size());
  while (!todo.empty()) {
    // get u and the distance to u from the queue
    auto [u, du] = todo.top();
    todo.pop();
    if (vis[u]) continue;
    vis[u] = true;
    for (auto [v, cost] : graph[u]) {
      // follow the edge u -> v if we get a shorter distance
      if (dist[v] == -1 || du + cost < dist[v]) {
        dist[v] = du + cost;
        todo.emplace(v, dist[v]);
      }
    }
  }
  return dist;
}

