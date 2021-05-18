/* Dinic (Max-Flow Min-Cut)
 * USAGE
 *  dinic<T> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, c, bidirectional?);
 *  T flow = graph.flow(source, sink);
 * MEMBERS
 *  add_edge(a, b, c, bidirectional?); adds an edge a -> b with capacity c.
 *    bidirectional? is a boolean specifying whether the edge is undirected.
 *    default is false
 *  flow(source, sink); returns max flow from source to sink.
 *  clear_flow(); clears the flow in the graph
 * TIME
 *  O(EV^2) in general
 *  O(E min(V^{2/3}, sqrtE)) for unit caps
 *  O(EsqrtV) for bipartite matchings (bipartite + unit caps)
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: kattis/maxflow,hubtown,risk,sandart, cf/100199b
 */
#pragma once

template <typename T>
struct dinic {
  struct edge {
    int to, rev;
    T cap, flow;
    edge(int t, int r, const T& c, const T& f): to(t), rev(r), cap(c), flow(f) {}
  };
  vector<vector<edge>> adj;
  vector<int> layer, cur_edge;
  dinic(int n): adj(n), layer(n), cur_edge(n) {}

  void add_edge(int a, int b, const T& c, bool bidirectional = false) {
    adj[a].emplace_back(b, (int)size(adj[b]), c, 0);
    adj[b].emplace_back(a, (int)size(adj[a]) - 1, bidirectional ? c : 0, 0);
  }
  void clear_flow() {
    for (vector<edge>& adjacency : adj) {
      for (edge& e : adjacency) {
        e.flow = 0;
      }
    }
  }

  bool bfs(int s, int t) {
    fill(begin(layer), end(layer), -1);
    fill(begin(cur_edge), end(cur_edge), 0);
    queue<int> todo;
    layer[s] = 0;
    todo.push(s);
    while (!empty(todo) && layer[t] == -1) {
      int u = todo.front();
      todo.pop();
      for (const edge& e : adj[u]) {
        if (layer[e.to] == -1 && e.flow < e.cap) {
          layer[e.to] = layer[u] + 1;
          todo.push(e.to);
        }
      }
    }
    return layer[t] != -1;
  }

  T dfs(int u, int t, T f) {
    if (u == t || f == 0) return f;
    T res = 0;
    for (int i = cur_edge[u]; i < (int)size(adj[u]) && f != 0; i++) {
      cur_edge[u] = i; // save current edge
      if (layer[u] + 1 != layer[adj[u][i].to]) continue;
      if (T cur_flow = dfs(adj[u][i].to, t, min(f, adj[u][i].cap - adj[u][i].flow))) {
        adj[u][i].flow += cur_flow;
        adj[adj[u][i].to][adj[u][i].rev].flow -= cur_flow;
        res += cur_flow;
        f -= cur_flow;
      }
    }
    return res;
  }

  T flow(int source, int sink, int max_iters = numeric_limits<int>::max()) {
    if constexpr (!is_integral_v<T>) {
      assert(max_iters < numeric_limits<int>::max());
    }
    T inf = numeric_limits<T>::max();
    T res = 0;
    for (int i = 0; i < max_iters && bfs(source, sink); i++) {
      while (T cur_flow = dfs(source, sink, inf)) {
        res += cur_flow;
      }
    }
    return res;
  }
};

