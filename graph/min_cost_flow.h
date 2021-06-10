/* Minimum Cost Maximum Flow
 * USAGE
 *  min_cost_flow<T> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, cap, cost);
 *  pair<T, T> flow_cost = graph.flow(source, sink);
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

template <typename T, bool sparse = true>
struct min_cost_flow {
  static constexpr T inf = numeric_limits<T>::max() / 4;
  struct edge {
    int to, rev;
    T cap, flow, cost;
    edge(int t, int r, const T& c, const T& f, const T& d):
      to(t), rev(r), cap(c), flow(f), cost(d) {}
  };
  vector<vector<edge>> adj;
  vector<T> pot, dist;
  vector<int> vis, path;
  bool has_negative;
  min_cost_flow(int n):
    adj(n), pot(n), dist(n), vis(n), path(n), has_negative(false) {}

  void add_edge(int a, int b, const T& cap, const T& cost) {
    adj[a].emplace_back(b, (int)size(adj[b]), cap, 0, cost);
    adj[b].emplace_back(a, (int)size(adj[a]) - 1, 0, 0, -cost);
    has_negative |= (cost < 0);
  }
  void init_potential() {
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

  pair<T, T> augmenting_path(int source, int sink) {
    fill(begin(vis), end(vis), false);
    fill(begin(dist), end(dist), inf);
    dist[source] = 0;
    if constexpr (sparse) {
      priority_queue<pair<T, int>, vector<pair<T, int>>, greater<>> dijk;
      dijk.emplace(0, source);
      while (!empty(dijk)) {
        auto [d, u] = dijk.top();
        dijk.pop();
        if (vis[u]) continue;
        vis[u] = true;
        for (const edge& e : adj[u]) {
          if (e.flow < e.cap && dist[u] + pot[u] - pot[e.to] + e.cost < dist[e.to]) {
            dist[e.to] = dist[u] + pot[u] - pot[e.to] + e.cost;
            path[e.to] = e.rev;
            dijk.emplace(dist[e.to], e.to);
          }
        }
      }
    } else { // dense O(V^2 + E) dijkstra instead of O(ElogV)
      while (true) {
        pair<T, int> best(inf, -1);
        for (int i = 0; i < (int)adj.size(); i++) {
          if (!vis[i]) {
            best = min(best, pair(dist[i], i));
          }
        }
        if (best.second == -1) break;
        int u = best.second;
        vis[u] = true;
        for (const edge& e : adj[u]) {
          if (e.flow < e.cap && dist[u] + pot[u] - pot[e.to] + e.cost < dist[e.to]) {
            dist[e.to] = dist[u] + pot[u] - pot[e.to] + e.cost;
            path[e.to] = e.rev;
          }
        }
      }
    }
    if (dist[sink] == inf) {
      return pair(0, 0);
    }
    for (size_t i = 0; i < size(adj); i++) {
      pot[i] = min(inf, dist[i] + pot[i]);
    }

    T df = inf;
    T dcost = 0;
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
    return pair(df, dcost);
  }

  pair<T, T> flow(int source, int sink) {
    if (has_negative) init_potential();
    T res = 0;
    T cost = 0;
    auto [df, dcost] = augmenting_path(source, sink);
    while (df) {
      res += df;
      cost += dcost;
      tie(df, dcost) = augmenting_path(source, sink);
    }
    return pair(res, cost);
  }
};

