/* Push Relabel (Max-Flow)
 * USAGE
 *  push_relabel<T> graph(n); // n = number of vertices
 *  graph.add_edge(a, b, c, bidirectional?);
 *  graph.flow(source, sink);
 * MEMBERS
 *  add_edge(a, b, c, bidirectional?); adds an edge a -> b with capacity c.
 *    bidirectional? is a boolean specifying whether the edge is undirected.
 *    default is false
 *  flow(source, sink); returns max flow from source to sink.
 *  clear_flow(); clears the flow in the graph (untested)
 * TIME
 *  O(V^2sqrtE)
 *  V = #vertices, E = #edges
 * STATUS
 *  tested: kattis/maxflow,mincut, loj/127, spoj/FASTFLOW,FFLOW
 */
#pragma once

#include <numeric>
#include <queue>
#include <vector>

template <typename T>
struct push_relabel {
  static constexpr T inf = std::numeric_limits<T>::max();
  static constexpr bool is_push_relabel = true;
  struct edge {
    int to, rev;
    T cap, flow;
    edge(int t, int r, const T& c, const T& f) : to(t), rev(r), cap(c), flow(f) {}
  };
  std::vector<std::vector<edge>> adj;
  std::vector<std::vector<int>> active;
  std::vector<int> height, layer;
  std::vector<T> excess;
  int work;
  push_relabel(int n) : adj(n), active(n), height(n), layer(n), excess(n) {}
  void add_edge(int a, int b, const T& c, bool bidirectional = false) {
    adj[a].emplace_back(b, (int)adj[b].size(), c, 0);
    adj[b].emplace_back(a, (int)adj[a].size() - 1, bidirectional ? c : 0, 0);
  }
  bool left_of_min_cut(int u) const { return height[u] >= size(); }
  int size() const { return (int)adj.size(); }
  void clear_flow() {
    active.resize(size());
    for (int i = 0; i < size(); i++) {
      for (edge& e : adj[i]) {
        e.flow = 0;
      }
      active[i].clear();
      height[i] = excess[i] = layer[i] = 0;
    }
  }

  int global_relabel(int sink, int cur_height = -1) {
    work = 0;
    fill(begin(height), end(height), size());
    fill(begin(layer), end(layer), 0);
    for (int i = 0; i <= cur_height; i++) {
      active[i].clear();
    }
    int max_height = 0;
    std::queue<int> bfs;
    bfs.push(sink);
    height[sink] = 0;
    layer[0] += 1;
    while (!empty(bfs)) {
      int u = bfs.front();
      bfs.pop();
      for (edge& e : adj[u]) {
        if (height[e.to] == size() && adj[e.to][e.rev].flow < adj[e.to][e.rev].cap) {
          bfs.push(e.to);
          height[e.to] = height[u] + 1;
          layer[height[e.to]] += 1;
          if (excess[e.to] > 0) {
            active[height[e.to]].push_back(e.to);
            max_height = height[e.to];
          }
        }
      }
    }
    return max_height;
  }

  void push(int u, edge& e) {
    if (excess[e.to] == 0) {
      active[height[e.to]].push_back(e.to);
    }
    T flow = std::min(excess[u], e.cap - e.flow);
    e.flow += flow;
    adj[e.to][e.rev].flow -= flow;
    excess[e.to] += flow;
    excess[u] -= flow;
  }

  int discharge(int u, bool backward = false) {
    int max_to = height[u];
    while (excess[u] > 0 && (backward || height[u] < size())) {
      work++;
      int relabel_height = backward ? 2 * size() : size();
      for (edge& e : adj[u]) {
        if (e.flow == e.cap) continue;
        if (height[e.to] >= height[u]) {
          relabel_height = std::min(relabel_height, height[e.to] + 1);
        } else {
          push(u, e);
          max_to = std::max(max_to, height[e.to]);
          if (excess[u] == 0) {
            break;
          }
        }
      }
      if (excess[u] == 0) break;
      if (!backward && layer[height[u]] == 0) {
        height[u] = size();  // disconnected from the sink
      } else {
        height[u] = relabel_height;
      }
    }
    if (height[u] < size()) {
      layer[height[u]] += 1;
    }
    return max_to;
  }

  T calc_flow(int source, int sink, int work_before_relabel) {
    height[source] = size();
    excess[source] = inf;
    excess[sink] = -inf;
    for (edge& e : adj[source]) {
      if (e.flow < e.cap) {
        push(source, e);
      }
    }
    layer[0] = size() - 1;
    for (int cur = work = 0; cur >= 0; cur--) {
      while (!empty(active[cur])) {
        int u = active[cur].back();
        active[cur].pop_back();
        layer[cur] -= 1;
        cur = discharge(u);
        if (work > work_before_relabel * size()) {
          cur = global_relabel(sink, cur);
        }
      }
    }
    return excess[sink] + inf;
  }

  T flow(int source, int sink, bool need_flow = false, int work_before_relabel = 8) {
    if (!need_flow) return calc_flow(source, sink, work_before_relabel);
    T res = calc_flow(source, sink, work_before_relabel);
    global_relabel(sink);
    active.resize(2 * size());
    excess[source] = -inf;
    for (int i = 0; i < size(); i++) {
      if (i != source && excess[i] > 0) {
        active[size()].push_back(i);
      }
    }
    for (int cur = size(); cur >= size(); cur--) {
      while (!empty(active[cur])) {
        int u = active[cur].back();
        active[cur].pop_back();
        cur = discharge(u, true);
      }
    }
    return res;
  }
};
