/* Ford Fulkerson (Max-Flow)
 * USAGE
 *  ford_fulkerson<T> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, c, bidirectional?);
 *  T flow = graph.flow(source, sink);
 * MEMBERS
 *  add_edge(a, b, c, bidirectional?); adds an edge a -> b with capacity c.
 *    bidirectional? is a boolean specifying whether the edge is undirected.
 *    default is false
 *  flow(source, sink); returns max flow from source to sink.
 *  clear_flow(); clears the flow in the graph (untested)
 * TIME
 *  O(fE)
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: kattis/roomevacuation
 */
#pragma once

#include <cassert>
#include <limits>
#include <vector>

template <typename T>
struct ford_fulkerson {
  struct edge {
    int to, rev;
    T cap, flow;
    edge(int t, int r, const T& c, const T& f) : to(t), rev(r), cap(c), flow(f) {}
  };
  std::vector<std::vector<edge>> adj;
  std::vector<bool> visited;
  ford_fulkerson(int n) : adj(n), visited(n) {}

  int size() const { return (int)adj.size(); }
  void add_edge(int a, int b, const T& c = 1, bool bidirectional = false) {
    adj[a].emplace_back(b, (int)adj[b].size(), c, 0);
    adj[b].emplace_back(a, (int)adj[a].size() - 1, bidirectional ? c : 0, 0);
  }
  void clear_flow() {
    for (std::vector<edge>& adjacency : adj) {
      for (edge& e : adjacency) {
        e.flow = 0;
      }
    }
  }

  T dfs(int u, int t, T f) {
    visited[u] = true;
    for (int i = 0; i < (int)adj[u].size(); i++) {
      auto& e = adj[u][i];
      T pushable = std::min(f, e.cap - e.flow);
      if (!pushable || visited[e.to]) continue;
      if (T cur_flow = (e.to == t ? pushable : dfs(adj[u][i].to, t, pushable))) {
        e.flow += cur_flow;
        adj[e.to][e.rev].flow -= cur_flow;
        return cur_flow;
      }
    }
    return T(0);
  }

  T flow(int source, int sink, int max_iters = std::numeric_limits<int>::max()) {
    if constexpr (!std::is_integral_v<T>) {
      assert(max_iters < std::numeric_limits<int>::max());
    }
    T inf = std::numeric_limits<T>::max();
    T res = 0;
    for (int i = 0; i < max_iters; i++) {
      std::fill(visited.begin(), visited.end(), false);
      if (T cur_flow = dfs(source, sink, inf)) {
        res += cur_flow;
      } else {
        break;
      }
    }
    return res;
  }
};
