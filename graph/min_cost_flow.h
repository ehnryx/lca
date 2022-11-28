/* Minimum Cost Maximum Flow
 * USAGE
 *  min_cost_flow<Flow_t, Cost_t> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, cap, cost);
 *  pair<Flow_t, Cost_t> flow_cost = graph.flow(source, sink);
 * MEMBERS
 *  add_edge(a, b, cap, cost, bidirectional?);
 *    adds an edge a -> b with cap and cost.
 *  flow(source, sink); returns max flow from source to sink.
 *  clear_flow(); clears the flow in the graph (untested)
 * TIME
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: kattis/mincostmaxflow,excitingtournament
 */
#pragma once

#include <algorithm>
#include <numeric>
#include <queue>
#include <vector>

template <typename Flow_t, typename Cost_t, bool sparse = true>
struct min_cost_flow {
  static constexpr Flow_t flow_inf = std::numeric_limits<Flow_t>::max() / 4;
  static constexpr Cost_t cost_inf = std::numeric_limits<Cost_t>::max() / 4;
  struct edge {
    int to, rev;
    Flow_t cap, flow;
    Cost_t cost;
    edge(int t, int r, const Flow_t& c, const Flow_t& f, const Cost_t& d):
      to(t), rev(r), cap(c), flow(f), cost(d) {}
  };
  std::vector<std::vector<edge>> adj;
  std::vector<Cost_t> pot, dist;
  std::vector<int> vis, path;
  bool has_negative;
  min_cost_flow(int n):
    adj(n), pot(n), dist(n), vis(n), path(n), has_negative(false) {}

  void add_edge(int a, int b, const Flow_t& cap, const Cost_t& cost) {
    adj[a].emplace_back(b, (int)size(adj[b]), cap, 0, cost);
    adj[b].emplace_back(a, (int)size(adj[a]) - 1, 0, 0, -cost);
    has_negative |= (cost < 0);
  }

  void init_potential() {
    std::fill(begin(pot), end(pot), 0);
    for (size_t i = 1; i < adj.size(); i++) {
      for (size_t u = 0; u < adj.size(); u++) {
        for (const edge& e : adj[u]) {
          if (e.cap > 0) {
            pot[e.to] = min(pot[e.to], pot[u] + e.cost);
          }
        }
      }
    }
  }

  void clear_flow() {
    for (std::vector<edge>& adjacency : adj) {
      for (edge& e : adjacency) {
        e.flow = 0;
      }
    }
  }

  std::pair<Flow_t, Cost_t> augmenting_path(int source, int sink) {
    std::fill(begin(vis), end(vis), false);
    std::fill(begin(dist), end(dist), cost_inf);
    dist[source] = 0;
    if constexpr (sparse) {
      std::priority_queue<
        std::pair<Cost_t, int>,
        std::vector<std::pair<Cost_t, int>>,
        std::greater<>> dijk;
      dijk.emplace(0, source);
      while (!empty(dijk)) {
        auto [d, u] = dijk.top();
        dijk.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (const edge& e : adj[u]) {
          if (!vis[e.to] && e.flow < e.cap &&
              dist[u] + pot[u] - pot[e.to] + e.cost < dist[e.to]) {
            dist[e.to] = dist[u] + pot[u] - pot[e.to] + e.cost;
            path[e.to] = e.rev;
            dijk.emplace(dist[e.to], e.to);
          }
        }
      }
    } else { // dense O(V^2 + E) dijkstra instead of O(ElogV)
      while (true) {
        std::pair<Cost_t, int> best(cost_inf, -1);
        for (int i = 0; i < (int)adj.size(); i++) {
          if (!vis[i]) {
            best = min(best, std::pair(dist[i], i));
          }
        }
        if (best.second == -1) break;
        int u = best.second;
        vis[u] = true;
        for (const edge& e : adj[u]) {
          if (!vis[e.to] && e.flow < e.cap &&
              dist[u] + pot[u] - pot[e.to] + e.cost < dist[e.to]) {
            dist[e.to] = dist[u] + pot[u] - pot[e.to] + e.cost;
            path[e.to] = e.rev;
          }
        }
      }
    }
    if (dist[sink] == cost_inf) {
      return std::pair(0, 0);
    }
    for (size_t i = 0; i < size(adj); i++) {
      pot[i] = min(cost_inf, dist[i] + pot[i]);
    }

    Flow_t df = flow_inf;
    Cost_t dcost = 0;
    for (int u = sink; u != source; u = adj[u][path[u]].to) {
      const edge& back = adj[u][path[u]];
      const edge& e = adj[back.to][back.rev];
      df = min(df, e.cap - e.flow);
    }
    for (int u = sink; u != source; u = adj[u][path[u]].to) {
      edge& back = adj[u][path[u]];
      edge& e = adj[back.to][back.rev];
      e.flow += df;
      back.flow -= df;
      dcost += df * e.cost;
    }
    return std::pair(df, dcost);
  }

  std::pair<Flow_t, Cost_t> flow(int source, int sink) {
    if (has_negative) init_potential();
    Flow_t res = 0;
    Cost_t cost = 0;
    auto [df, dcost] = augmenting_path(source, sink);
    while (df) {
      res += df;
      cost += dcost;
      tie(df, dcost) = augmenting_path(source, sink);
    }
    return std::pair(res, cost);
  }
};

