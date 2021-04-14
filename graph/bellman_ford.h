/* Bellman Ford
 * USAGE
 *  auto [dist, neg_cycle] = bellman_ford(edges, n, source, inf, reachable);
 * INPUT
 *  edges: list of edges of a weighted directed graph
 *    edges[i] is a tuple (a, b, c) representing an edge a -> b with cost c
 *  n: number of vertices
 *  source: the vertex from which to find distances
 *  inf: infinity, should satisfy n * cost < inf and inf + inf < overflow.
 *  reachable: whether negative cycles should be reachable from s
 * OUTPUT
 *  dist[i] = distance from source to i
 *  dist[i] = inf if it is impossible to reach i from source
 *  neg_cycle = true if there is a negative cycle
 *    if reachable is true, then only detects negative cycles reachable from s
 *    otherwise, detects any negative cycles in the graph
 * TIME
 *  O(VE)
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: kattis/shortestpath3
 */
#pragma once

template <typename T>
pair<vector<T>, bool> bellman_ford(
    const vector<tuple<int, int, T>>& edges,
    int n,
    int source,
    int inf,
    bool reachable = true) {
  vector<T> dist(n, inf);
  dist[source] = 0;
  for (int i = 1; i < n; i++) {
    for (auto [a, b, cost] : edges) {
      if (!reachable || dist[a] < inf) {
        dist[b] = min(dist[b], dist[a] + cost);
      }
    }
  }
  bool neg = false;
  for (auto [a, b, cost] : edges) {
    if ((!reachable || dist[a] < inf) && dist[a] + cost < dist[b]) {
      neg = true;
      break;
    }
  }
  return pair(dist, neg);
}

